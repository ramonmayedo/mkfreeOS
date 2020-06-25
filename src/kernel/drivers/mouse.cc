/*Copyright (C) 2019  Ramón Mayedo Morales (ramonmayedo@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "mouse.h"
#include "../architecture/x86/x86.h"
extern Sx86 x86;
extern Smaps maps;
extern Score core;

CmausePS2::CmausePS2() {
    waitWrite(maps.mouseMap.port.regControl, 0xA8);
    waitWrite(maps.mouseMap.port.regControl, 0x20);
    u8 status = waitRead(maps.mouseMap.port.regBase) | 2;
    waitWrite(maps.mouseMap.port.regControl, maps.mouseMap.port.regBase);
    waitWrite(maps.mouseMap.port.regBase, status);
    write(0xf6);
    ack();
    write(0xF4);
    ack();
    maps.mouseMap.state = 0;
    state = 0;
    process = 0;    
}

void CmausePS2::IRQ() {
    u8 status = x86.port.inb(maps.mouseMap.port.regControl);
    if ((status & 0x20)&& (status & 0x1)) {
        int data = read();
        switch (maps.mouseMap.state) {
            case 0:
            {
                maps.mouseMap.flags = data;
                maps.mouseMap.buttons = (maps.mouseMap.flags & 0x7);
                maps.mouseMap.state++;
                break;
            }
            case 1:
            {
                maps.mouseMap.dx = data;
                maps.mouseMap.state++;
                break;
            }
            case 2:
            {
                maps.mouseMap.dy = data;
                int x = maps.mouseMap.dx;
                int y = maps.mouseMap.dy;
                bool x_overflow = maps.mouseMap.flags & 0x40;
                bool y_overflow = maps.mouseMap.flags & 0x80;
                bool x_sign = maps.mouseMap.flags & 0x10;
                bool y_sign = maps.mouseMap.flags & 0x20;
                if (x && x_sign)
                    x -= 0x100;
                if (y && y_sign)
                    y -= 0x100;
                if (x_overflow || y_overflow) {
                    x = 0;
                    y = 0;
                }

                 maps.mouseMap.dx = x;
                 maps.mouseMap.dy = y;
                
                maps.mouseMap.state = 0;
                buffer[count] = maps.mouseMap.buttons;
                
                if (count < 31) count++;
                if (process != 0) {
                    core.adminProcess.unlockProcess(process, thread);
                    process = 0;
                }
                break;
            }
        }
    }
    x86.pic8259.reconocer();
}

int CmausePS2::command(int acommand, int parameter1, int parameter2) {
    Cprocess *processRun = core.adminProcess.getRun();
    switch (acommand) {
        case cmmMouseEvent:
        {
            if (state == cmdMouseBlock) { //si esta bloqueado el mause
                unblock(); //se desbloquea 
            } else /*if (count == 0)*/ {
                ptr = 0;
                block(); //Se bloquea el mouse
                processRun->sendState(cmdMouseSleep); //se suspende el proceso a espera de un evento del mouse
                return 0;
            }
            if (ptr == count - 1) count = 0;
            *(int*)parameter1 = maps.mouseMap.dx;
            *(int*)parameter2 = maps.mouseMap.dy;
            return maps.mouseMap.buttons;//buffer[ptr++];
        }
        default:break;
    }
}

int CmausePS2::getState() {
    return state;
}

int CmausePS2::block() {
    state = cmdMouseBlock;
    process = core.adminProcess.getRun();
    thread = process->adminThread->getRun();
}

int CmausePS2::unblock() {
    state = 0;
}

u8 CmausePS2::waitRead(u32 port) {
    waitInPut();
    return x86.port.inb(port);
}

void CmausePS2::waitWrite(u32 port, u8 value) {
    waitOutPut();
    x86.port.outb(port, value);
}

void CmausePS2::waitOutPut() {
    while ((x86.port.inb(maps.mouseMap.port.regControl) & 2));
}

void CmausePS2::waitInPut() {
    while (!(x86.port.inb(maps.mouseMap.port.regControl) & 1));
}

void CmausePS2::write(u8 data) {
    waitOutPut();
    x86.port.outb(maps.mouseMap.port.regControl, 0xD4);
    waitOutPut();
    x86.port.outb(maps.mouseMap.port.regBase, data);
}

u8 CmausePS2::read() {
    waitInPut();
    return x86.port.inb(maps.mouseMap.port.regBase);
}

void CmausePS2::ack() {
    u8 data = read();
}
