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
     setDirectory((u32)VM_KERNEL_PAGE_DIR);
}

void Carchitecture::changeToDirectoryCurrentProcess() {
    setDirectory((u32)core.adminProcess.getRun()->process->processX86.cr3);
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

void Carchitecture::kernelStopScreen(char *stringFault) {
    x86.ioScreen.clearScreen();
    x86.ioScreen.printf("*****************KENEL STOP SCREEN**********************/n/n");
    x86.ioScreen.printf("%s", stringFault);
    x86.ioScreen.printf("/n Register Status /n");
    x86.ioScreen.printf("/n         CS = %h : ", maps.statusX86.cs);
    x86.ioScreen.printf("EIP = %h /n", maps.statusX86.eip);
    x86.ioScreen.printf("/n         CR2 = %h /n", maps.statusX86.cr2);
    x86.ioScreen.printf("/n*********************KENEL STOP************************/n/n");
    while (1);
}
