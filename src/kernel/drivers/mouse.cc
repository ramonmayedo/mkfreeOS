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
extern Score core;

CmausePS2::CmausePS2() {

}

void CmausePS2::initialize() {
    waitWrite(MS_CONTROL_REGISTER, 0xA8);
    waitWrite(MS_CONTROL_REGISTER, 0x20);
    u8 status = waitRead(MS_BASE_REGISTER) | 2;
    waitWrite(MS_CONTROL_REGISTER, MS_BASE_REGISTER);
    waitWrite(MS_BASE_REGISTER, status);
    write(0xf6);
    ack();
    write(0xF4);
    ack();
    mouseMap.state = 0;
    ptr = 0;
    count = 0;
    state = cmdMouseLock;
    locksProcess.initialize();
    lockProcess = 0;
}

void CmausePS2::IRQ() {
    u8 status = x86.port.inb(MS_CONTROL_REGISTER);
    if ((status & 0x20)&& (status & 0x1)) {
        int data = read();
        switch (mouseMap.state) {
            case 0:
            {
                mouseMap.flags = data;
                mouseMap.buttons = (mouseMap.flags & 0x7);
                mouseMap.state++;
                break;
            }
            case 1:
            {
                mouseMap.dx = data;
                mouseMap.state++;
                break;
            }
            case 2:
            {
                mouseMap.dy = data;
                int x = mouseMap.dx;
                int y = mouseMap.dy;
                bool x_overflow = mouseMap.flags & 0x40;
                bool y_overflow = mouseMap.flags & 0x80;
                bool x_sign = mouseMap.flags & 0x10;
                bool y_sign = mouseMap.flags & 0x20;
                if (x && x_sign)
                    x -= 0x100;
                if (y && y_sign)
                    y -= 0x100;
                if (x_overflow || y_overflow) {
                    x = 0;
                    y = 0;
                }

                mouseMap.dx = x;
                mouseMap.dy = y;
                mouseMap.state = 0;
                buffer[count] = mouseMap.buttons;
                if (count < 31) count++;
                    unlock();
                break;
            }
        }
    }
}

int CmausePS2::command(int acommand, int parameter1, int parameter2) {
    switch (acommand) {
        case cmmMouseEvent:
        {
            if (state == cmdMouseLock) { //si esta bloqueado ya elmouse
               confirmUnlock();               //se bloquea 
              if (state == cmdMouseLock)
               lock(); //Si esta bloqueado
            }  
            if (count == 0) {     //Si no hay que leer se bloquea
                ptr = 0;
                lock(); //Se bloquea el mouse
            }
            if (ptr == count - 1) count = 0;
            ptr++;
            *(int*) parameter1 = mouseMap.dx;
            *(int*) parameter2 = mouseMap.dy;
            return mouseMap.buttons;
        }
        default:break;
    }
}

int CmausePS2::getState() {
    return state;
}

int CmausePS2::lock() {
    state = cmdMouseLock;
    SlockProcess *lockProcess = new SlockProcess;
    lockProcess->thread = core.adminProcess.getRun();
    lockProcess->state = cmdMouseLock;
    locksProcess.add(lockProcess);
    lockProcess->thread->sendState(cmdMouseSleep, 60); //se suspende el proceso a espera de una tecla

}

int CmausePS2::unlock() {
    while (locksProcess.count()) {
        lockProcess = (SlockProcess*) locksProcess.removeFirst(); //Tomo el primero de la lista
        if (core.adminProcess.unlockThread(lockProcess->thread, ADP_PRIORITY_HIGHT) != -1)
            break;
        delete(lockProcess); //Se libera la memoria ya que no existe este proceso
        lockProcess = 0; //Se indica que no hay proceso 
    }
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
    while ((x86.port.inb(MS_CONTROL_REGISTER) & 2));
}

void CmausePS2::waitInPut() {
    while (!(x86.port.inb(MS_CONTROL_REGISTER) & 1));
}

void CmausePS2::write(u8 data) {
    waitOutPut();
    x86.port.outb(MS_CONTROL_REGISTER, 0xD4);
    waitOutPut();
    x86.port.outb(MS_BASE_REGISTER, data);
}

u8 CmausePS2::read() {
    waitInPut();
    return x86.port.inb(MS_BASE_REGISTER);
}

void CmausePS2::ack() {
    u8 data = read();
}

int CmausePS2::confirmUnlock() {
    if (lockProcess) {
        //Si fue liberado por el hilo que se ejecuta 
        if (lockProcess->thread == core.adminProcess.getRun()) {
            state = cmdUnlock;   //Se desbloquea el teclado
            delete(lockProcess); //Se libera la memoria
            lockProcess = 0;     //Se indica que no hay procesos
            return 0;
        }            //Si el proceso fue eliminado, muevo la cola
        else if (core.adminProcess.isThreadExist(lockProcess->thread) == -1) {
            delete(lockProcess); 
            unlock();             //Se busca otro proceso para poner en listos
        }            //Si fue eliminado el hilo, muevo la cola
       /* else if (lockProcess->process->adminThread->isThreadExist(lockProcess->thread) == -1) {
            delete(lockProcess); //Se busca otro proceso para poner en listos
            unlock();
        }*/
        //Si ya no hay procesos listos desbloqueo el teclado
        if (lockProcess == 0)
            state = cmdUnlock;
    }
    return 0;
}