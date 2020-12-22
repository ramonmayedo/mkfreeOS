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

#include "cdide.h"
#include "../architecture/x86/x86.h"

extern Smaps maps;
extern Sx86 x86;

/* READ (12) Command
Byte/ Bit     7    6    5    4    3    2    1    0
 0            Operation Code (A8h)
 1            Reserved
 2                   MSB
 3            Logical Block Address
 4                   
 5                   LSB
 6                   MSB
 7            Transfer Length
 8
 9                   LSB
 10           Reserved
 11           Reserved 
 */
Ccdide::Ccdide() {

}

int Ccdide::waitDisk(SdevicePort *device) {
    while (x86.port.inb(device->regControl) & 0x80); //Registro de control
    return 0;
}

int Ccdide::detectDisk(SdevicePort* device, u8 deviceIndex) {
    u8 mask = (u8) (0xA | (deviceIndex << 4));
    x86.port.outb(device->regBase + 0x6, mask);
    delay400ns(device);
    x86.port.outb(device->regBase + 0x2, 0);
    x86.port.outb(device->regBase + 0x3, 0);
    x86.port.outb(device->regBase + 0x4, 0);
    x86.port.outb(device->regBase + 0x5, 0);
    x86.port.outb(device->regBase + 0x7, 0xEC);
    while (x86.port.inb(device->regControl) & 0x80);
    return 0;
}

void Ccdide::initCommand(SdevicePort *device, u8 driveIndex) {
    waitDisk(device);
    //Selecciono el dispositivo
    u8 mask = (u8) (driveIndex & (1 << 4));
    x86.port.outb(device->regBase + 0x6, mask);
    delay400ns(device);
    //Selecciono modo PIO
    x86.port.outb(device->regBase + 0x1, 0x0);
    //Tamaño maximo de un sector de un disco en byte
    x86.port.outb(device->regBase + 0x4, (u8) (2048 & 0xFF));
    //Tamaño maximo de un sector de un disco en byte
    x86.port.outb(device->regBase + 0x5, (u8) (2048 >> 8));
}

int Ccdide::readSector(SdevicePort *device, u8 deviceIndex, u32 ablock, u32 asectorCount, char *abuffer) {
    //Bloque de Comando SCSI 0xA8 Lectura de CD
    u8 readCmd[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //x86.architecture.deseableInterruption();
    initCommand(device, deviceIndex);
    waitDisk(device);
    x86.port.outb(device->regBase + 0x7, 0xA0);
    //Comando 
    int status = x86.port.inb(device->regBase + 0x7)&0x1;

    while ((status = x86.port.inb(device->regBase + 0x7))&0x80);

    while (!((status = x86.port.inb(device->regBase + 0x7))&0x08)&&!(status & 0x1));
    if (status & 0x1) return 0;

    readCmd[9] = 1; //Cantidad de Sectores
    readCmd[2] = u8((ablock >> 24) & 0xFF); //Byte mas significativo LBA
    readCmd[3] = u8((ablock >> 16) & 0xFF);
    readCmd[4] = u8((ablock >> 8) & 0xFF);
    readCmd[5] = u8(ablock & 0xFF); //Byte menos significativo LBA

    u16 *val = (u16*) & readCmd;
    for (int i = 0; i < 6; i++) x86.port.outw(device->regBase, val[i]);

    while (x86.port.inb(device->regBase + 0x7)&0x80);  

    maps.ticksKernelDelay = 1;

    while (!(x86.port.inb(device->regBase + 0x7)&0x08) && (maps.ticksKernelDelay));
    
    if (maps.ticksKernelDelay == 0) return 0;
    
    int size = (int(x86.port.inb(device->regBase + 0x5) << 8) | int(x86.port.inb(device->regBase + 0x4)));
    
    if (size != 2048)  return 0;

    u16 auxWord;
    for (int i = 0; i < size / 2; i++) {
        auxWord = x86.port.inw(device->regBase);
        abuffer[i * 2] = (u8) auxWord;
        abuffer[i * 2 + 1] = (u8) (auxWord >> 8);
    }
    return asectorCount;
}

void Ccdide::delay400ns(SdevicePort *device) {
    for (int i = 0; i < 4; i++)
        x86.port.inb(device->regControl); //400ns  
}