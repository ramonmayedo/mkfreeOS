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

#ifndef CACHEDISK_H
#define CACHEDISK_H
#include "../defines/types.h"
#include "../architecture/x86/includes/maps.h"
#include "devices.h"
#define FLAG_USED 0x1
#define FLAG_MODY 0x2
#define MAX_ENTRY 512


struct Scache{
    u8  flags;
    u32 ptrMem;
    u32 idResorce;
    u32 old;
    u32 did;
};

class CcacheDisk {
public:
    CcacheDisk();
    int readSector(u32 adevice, u32 ablock, u32 asectorCount, char *abuffer);
    int writeSector(u32 adevice,u32 ablock,u32 asectorCount,char *abuffer);
    void flush();
private:
    Scache *table;
protected:
    int readSector(u32 adevice, u32 ablock, char *abuffer);
    int writeSector(u32 adevice,u32 ablock,char *abuffer);
};

#endif

