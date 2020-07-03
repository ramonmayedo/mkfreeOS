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

#ifndef DEVICES_H
#define DEVICES_H
#include "../defines/types.h"
#include "../architecture/x86/includes/maps.h"

#define DEV_ATA_UNKNOWN 0x0
#define DEV_ATA_PATAPI  0x1
#define DEV_ATA_SATAPI  0x2
#define DEV_ATA_PATA    0x3
#define DEV_ATA_SATA    0x4

struct Sdevice {
    int did; //Identificador unico del dispositivo
    u8 type; //Tipo de didpositivo
    u8 flags; //Banderas
    u8 index; //Index
    SdevicePort *port;
} __attribute__((packed));

class Cdevices {
public:
    Cdevices();
    int installDevices();
    u8 readDevices(SdevicePort *device, u8 devIndex);
    Sdevice *getDevice(int did);
    int getCountDevice();
    int readDevice(u32 did, u32 ablock, u32 countSector, char *abuffer);
    int writeDevice(u32 did, u32 ablock, u32 countSector, char *abuffer);
private:
    Sdevice devices[10]; //Tabla de dispositivos instalados
    u32 countDevices;
    void resetBus(SdevicePort *device);
    void delay400ns(SdevicePort *device);
};

#endif

