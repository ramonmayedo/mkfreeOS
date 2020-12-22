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

#include "kernel.h"
#include "../architecture/x86/x86.h"
#include "../architecture/x86/smp.h"
#include "../architecture/x86/ioapic.h"

extern Sx86 x86;
extern Score core;
extern Smaps maps;

void kernel() {   
    x86.ioScreen.printf("Ya estamos en el kernel MKFREE h:%i m: %i s: %i /n", maps.clockMap.hour, maps.clockMap.minute, maps.clockMap.second);
    x86.ioScreen.printf("Ya estamos en el kernel MKFREE y:%i m: %i d: %i /n", maps.clockMap.year, maps.clockMap.month, maps.clockMap.day);
        
    for (int i = 0; i < core.devices.getCountDevice(); i++)
        core.diskPartition.mountFileSystem(i); //Se montan los sistemas de archivo

    int volSel = core.fileSystem.selectVolume('a');

    if (volSel != -1) {
        x86.ioScreen.printf("Volumen seleccionado [a] /n"); 
        SinfoMedia *infoMedia = core.fileSystem.getInfoVolume();
        x86.ioScreen.printf("label=%s /n", infoMedia->labelVolume);
        x86.ioScreen.printf("type=%s /n", infoMedia->labelType);
        x86.ioScreen.printf("oem=%s /n", infoMedia->labelOem);
        x86.ioScreen.printf("size=%i MB /n", infoMedia->size / 1048576);
    }

    Cfile *newfile;
    char *buffer;
    char **arg;

    newfile = new Cfile("a/kernel/program/init.elf", FILE_RW);

    buffer = new char[newfile->getSize()];
    newfile->readAll(buffer);

    int error = core.adminProcess.addReady(buffer, 0, arg, priorityVeryLow);
    delete(buffer);
    delete(newfile);
    if (error) {
        core.pioscreen.connectScreen();
        core.adminProcess.schelude();
    }
    x86.architecture.kernelStopScreen("KERNEL Falta el proceso 0 init /n"); //Siempre debe haber un proceso ejecutandose
    while (1);
}