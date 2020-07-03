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

#include "cacheDisk.h"
#include "../architecture/x86/x86.h"
extern Sx86 x86;
extern Score core;

CcacheDisk::CcacheDisk() {

}

void CcacheDisk::initialize() {
    table512 = new Scache[MAX_ENTRY512];
    for (int i = 0; i < MAX_ENTRY512 / 8; i++) {
        u32 dirPtr = (u32) x86.virtualMemory.getPagesVirtualKernel();
        for (int j = 0; j < 8; j++) {
            table512[i * 8 + j].ptrMem = dirPtr + 512 * j;
            table512[i * 8 + j].flags = 0;
        }
    }

    table2048 = new Scache[MAX_ENTRY2048];
    for (int i = 0; i < MAX_ENTRY2048 / 2; i++) {
        u32 dirPtr = (u32) x86.virtualMemory.getPagesVirtualKernel();
        table2048[i * 2].ptrMem = dirPtr;
        table2048[i * 2].flags = 0;
        table2048[i * 2 + 1].ptrMem = dirPtr + 2048;
        table2048[i * 2 + 1].flags = 0;
    }
}

int CcacheDisk::readSector(u32 adevice, u32 ablock, char *abuffer) {
    int indexEmpty = 1;
    bool freeCache = false;
    u32 minEntry = 0;
    for (int i = 0; i < MAX_ENTRY512; i++) {
        if (table512[i].flags != 0) {
            if (table512[i].did == adevice && table512[i].idResorce == ablock) {
                core.memory.memcpy(abuffer, (char*) table512[i].ptrMem, 512);
                table512[i].old = 1; //Edad minima
                return i;
            } else if (freeCache == false) {
                if (minEntry < table512[i].old) { //Se escoje el mas viejo
                    minEntry = table512[i].old;
                    indexEmpty = i;
                }
                table512[i].old++; //se envejecen las entradas
            }
        } else {
            indexEmpty = i;
            freeCache = true;
        }
    }
    //Se remplaza el de mayor edad de la cache si no hay entradas disponibles
    //Si esta pagina se modifico y no se ha pasado al disco se salva en el disco 
    if (table512[indexEmpty].flags & FLAG_MODY)
        core.devices.writeDevice(table512[indexEmpty].did, table512[indexEmpty].idResorce, 1, (char*) table512[indexEmpty].ptrMem);
    if (abuffer) {
        if (core.devices.readDevice(adevice, ablock, 1, (char*) table512[indexEmpty].ptrMem) == 0) return ERROR_DEVICE;
        core.memory.memcpy(abuffer, (char*) table512[indexEmpty].ptrMem, 512);
    }
    table512[indexEmpty].did = adevice;
    table512[indexEmpty].idResorce = ablock;
    table512[indexEmpty].flags = FLAG_USED;
    table512[indexEmpty].old = 1; //edad minima
    return indexEmpty;
}

int CcacheDisk::readSectorExt(u32 adevice, u32 ablock, char *abuffer) {
    int indexEmpty = 1;
    bool freeCache = false;
    u32 minEntry = 0;
    for (int i = 0; i < MAX_ENTRY2048; i++) {
        if (table2048[i].flags != 0) {
            if (table2048[i].did == adevice && table2048[i].idResorce == ablock) {
                core.memory.memcpy(abuffer, (char*) table2048[i].ptrMem, 2048);
                table2048[i].old = 1; //Edad minima
                return i;
            } else if (freeCache == false) {
                if (minEntry < table2048[i].old) { //Se escoje el mas viejo
                    minEntry = table2048[i].old;
                    indexEmpty = i;
                }
                table2048[i].old++; //se envejecen las entradas
            }
        } else {
            indexEmpty = i;
            freeCache = true;
        }
    }
    //Se remplaza el de mayor edad de la cache si no hay entradas disponibles
    //Si esta pagina se modifico y no se ha pasado al disco se salva en el disco 
    if (table2048[indexEmpty].flags & FLAG_MODY)
        core.devices.writeDevice(table2048[indexEmpty].did, table2048[indexEmpty].idResorce, 1, (char*) table2048[indexEmpty].ptrMem);
    if (abuffer) {
        if (core.devices.readDevice(adevice, ablock, 1, (char*) table2048[indexEmpty].ptrMem) == 0) return ERROR_DEVICE;
        core.memory.memcpy(abuffer, (char*) table2048[indexEmpty].ptrMem, 2048);
    }

    table2048[indexEmpty].did = adevice;
    table2048[indexEmpty].idResorce = ablock;
    table2048[indexEmpty].flags = FLAG_USED;
    table2048[indexEmpty].old = 1; //edad minima
    return indexEmpty;
}

int CcacheDisk::writeSector(u32 adevice, u32 ablock, char *abuffer) {
    for (int i = 0; i < MAX_ENTRY512; i++) {
        if (table512[i].flags != 0) {
            if (table512[i].did == adevice && table512[i].idResorce == ablock) {
                core.memory.memcpy((char*) table512[i].ptrMem, abuffer, 512);
                table512[i].old = 1; //Edad minima
                table512[i].flags |= FLAG_MODY; //Se indica que se modifico
                return ablock;
            }
        }
    }
    //Si no esta en cache Se carga en cache sin leer
    int indexEmpty = readSector(adevice, ablock, 0);
    if (indexEmpty == ERROR_DEVICE) return 0;
    core.memory.memcpy((char*) table512[indexEmpty].ptrMem, abuffer, 512);
    table512[indexEmpty].flags |= FLAG_MODY;
}

void CcacheDisk::flush() {
    for (int i = 0; i < MAX_ENTRY512; i++) {
        if (table512[i].flags != 0) {
            if (table512[i].flags & FLAG_MODY) {
                char *ptr = (char*) table512[i].ptrMem;
                core.devices.writeDevice(table512[i].did, table512[i].idResorce, 1, ptr);
                table512[i].flags ^= FLAG_MODY;
            }
        }
    }
}

int CcacheDisk::readSector(u32 adevice, u32 ablock, u32 asectorCount, char *abuffer) {
    char *buffer = abuffer;
    u32 block = ablock;
    for (int i = 0; i < asectorCount; i++) {
        if (readSector(adevice, block, buffer) == ERROR_DEVICE) return 0;
        buffer += 512;
        block++;
    }
    return asectorCount;
}

int CcacheDisk::readSectorExt(u32 adevice, u32 ablock, u32 asectorCount, char *abuffer) {
    char *buffer = abuffer;
    u32 block = ablock;

    for (int i = 0; i < asectorCount; i++) {
        if (readSectorExt(adevice, block, buffer) == ERROR_DEVICE) return 0;
        buffer += 2048;
        block++;
    }
    return asectorCount;
}

int CcacheDisk::writeSector(u32 adevice, u32 ablock, u32 asectorCount, char *abuffer) {
    char *buffer = abuffer;
    u32 block = ablock;
    for (int i = 0; i < asectorCount; i++) {
        if (writeSector(adevice, block, buffer) == ERROR_DEVICE) return 0;
        buffer += 512;
        block++;
    }
    return asectorCount;
}