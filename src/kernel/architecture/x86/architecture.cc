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

#include "architecture.h"
#include "x86.h"

extern Smaps maps;
extern Sx86 x86;
extern Score core;

Carchitecture::Carchitecture() {
}

void Carchitecture::changeToDirectoryKernel(){
     setDirectory((u32)maps.memoryPagination.directoryPageKernel);
}

void Carchitecture::changeToDirectoryCurrentProcess() {
    setDirectory((u32)core.adminProcess.getRun()->processX86.regX86->cr3);
}

void Carchitecture::setDirectory(u32 adirectorty) {
    asm("movl %0,%%eax" : "=m"(adirectorty));
    asm("movl %eax,%cr3");
}

void Carchitecture::deseableInterruption() {
    asm("cli");
}

void Carchitecture::enabledInterruption() {
    asm("sti");
}

void Carchitecture::reset() {
    asm("cli");
    u8 good = 0x02;
    while ((good & 0x02) != 0)
        good = x86.port.inb(0x64);
    x86.port.outb(0x64, 0xFE);
}

void Carchitecture::shutDown() {
   /* asm("cli");
    u8 good = 0x02;
    while ((good & 0x02) != 0)
        good = x86.port.inb(0x64);*/
    x86.port.outw(0x604, 0x2000);
}

void Carchitecture::kernelStopScreen(int stopMode) {
    char string[10];
    x86.ioScreen.clearScreen();
    x86.ioScreen.printf("*****************KENEL STOP**********************/n/n");
    switch (stopMode) {
        case ARCHX86_GENERAL_FAULT:
        {
            x86.ioScreen.printf("/n              !!!! GENERAL FAULT !!!!             /n");
            break;
        }
        case ARCHX86_PAGE_FAULT:
        {
            x86.ioScreen.printf("                      !!!PAGE FAULT!!!!                        /n");
            break;
        }
        case ARCHX86_MEMORY_CORRUPT:
        {
            x86.ioScreen.printf("KERNEL malloc Error, Memoria Virtual Corrupta Size=0 /n");
            break;
        }
    }
    x86.ioScreen.printf("/n Register Status /n");
    core.conversion.IntToHexChar(maps.statusX86.cs, string, 4);
    x86.ioScreen.printf("/n         CS = %s : ", string);
    core.conversion.IntToHexChar(maps.statusX86.eip, string, 8);
    x86.ioScreen.printf("EIP = %s /n", string);
    x86.ioScreen.printf("/n*****************KENEL STOP**********************/n/n");
    while (1);
}
