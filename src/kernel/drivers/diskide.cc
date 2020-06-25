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

#include "diskide.h"
#include "../architecture/x86/x86.h"
extern Smaps maps;
extern Sx86 x86;

Cdiskide::Cdiskide() {
    
}

//CdiskidePIO::~CdiskidePIO() {
//}
/*  Registro            | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *  Features            |          n/a                  |
 *  Sector Count        |  Cantidad de sectores         |
 *  LBA Low             |           LBA(7:0)            |
 *  LBA Mid             |           LBA(15:8)           |
 *  LBA Hight           |           LBA(23:16)          |
 *  Device              |obs|LBA|obs|DEV|   LBA(27:24)  |
 *  Command             |           20h                 |
 * 
 * Sector Count: El numero que se puede tansferir
 * LBA         : Numero del sector
 * Device      : bit DEV selecciona dipositivo 0 o 1
 */
int Cdiskide::waitDisk(SdevicePort *device) {
    while (x86.port.inb(device->regControl) & 0x80);    //Registro de control  
    return 0;
}

void Cdiskide::initCommand(SdevicePort *device, u8 deviceIndex, u32 ablock, u32 asectorCount) {
    waitDisk(device);   //Se espera que el bus este desocupado
    x86.port.outb(device->regBase + 0x06, u8(0xA0 | (deviceIndex << 4)));  //Selecciono el dispositivo
    delay400ns(device);
    x86.port.outb(device->regBase + 0x1, 0x0);                 //Byte 0
    x86.port.outb(device->regBase + 0x2, asectorCount);        //byte 1
    x86.port.outb(device->regBase + 0x3, (u8) ablock);         //byte 2
    x86.port.outb(device->regBase + 0x4, (u8) (ablock >> 8));  //byte 3
    x86.port.outb(device->regBase + 0x5, (u8) (ablock >> 16)); //byte 4
    u8 mask = (u8) ((ablock & 0x0F000000) >> 24) | (deviceIndex) << 4 | 0xE0; //byte 5
    x86.port.outb(device->regBase + 0x6, mask);
}

int Cdiskide::readSector(SdevicePort *device, u8 deviceIndex, u32 ablock, u32 asectorCount, char *abuffer) {
    initCommand(device, deviceIndex, ablock, asectorCount);
    x86.port.outb(device->regBase + 0x7, 0x20); //byte 7 comando = 0x20 lectura PIO
    waitDisk(device);
    while (!(x86.port.inb(device->regBase + 0x7)&0x08));
    u16 auxWord;
    for (int i = 0; i < 256 * asectorCount; i++) {
        auxWord = x86.port.inw(device->regBase);
        abuffer[i * 2] = (u8) auxWord;
        abuffer[i * 2 + 1] = (u8) (auxWord >> 8);
    }
    return asectorCount;
}

int Cdiskide::writeSector(SdevicePort *device, u8 deviceIndex, u32 ablock, u32 asectorCount, char *abuffer) {
    initCommand(device, deviceIndex, ablock, asectorCount);
    x86.port.outb(device->regBase + 0x7, 0x30);            //Comando para escritura del disco       
    waitDisk(device);
    while (!(x86.port.inb(device->regBase + 0x7)&0x08));
    u16 auxWord;
    for (int i = 0; i < 256 * asectorCount; i++) {
        auxWord = u8(abuffer[i * 2 + 1]) << 8 | u8(abuffer[i * 2]);
        x86.port.outw(device->regBase, auxWord);
    }
    return asectorCount;
}

void Cdiskide::delay400ns(SdevicePort *device) {
    for (int i = 0; i < 4; i++)
        x86.port.inb(device->regControl); //400ns  
}

int Cdiskide::readSectorDMA(SdevicePort *device, u8 deviceIndex, u32 ablock, u32 asectorCount, char *abuffer){
    
}