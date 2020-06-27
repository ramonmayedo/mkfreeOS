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
    table = new Scache[MAX_ENTRY];
    for (int i = 0; i < MAX_ENTRY / 8; i++) {
        u32 dirPtr = (u32) x86.virtualMemory.getPagesVirtualKernel();
        for (int j = 0; j < 8; j++) {
            table[i * 8 + j].ptrMem = dirPtr + 512 * j;
            table[i * 8 + j].flags = 0;
        }
    }
}


int CcacheDisk::readSector(u32 adevice, u32 ablock, char *abuffer) {
    int indexEmpty = 0;
    bool freeCache = false;
    u32 minEntry = 0;
    for (int i = 0; i < MAX_ENTRY; i++) {
        if (table[i].flags != 0) {
            if (table[i].did == adevice && table[i].idResorce == ablock) {
                core.memory.memcpy(abuffer, (char*) table[i].ptrMem, 512);
                table[i].old = 1; //Edad minima
                return i;
            } else if (freeCache == false) {
                if (minEntry < table[i].old) { //Se escoje el mas viejo
                    minEntry = table[i].old;
                    indexEmpty = i;
                }
                table[i].old++; //se envejecen las entradas
            }
        } else {
            indexEmpty = i;
            freeCache = true;
        }
    }
    //Se remplaza el de mayor edad de la cache si no hay entradas disponibles
    //Si esta pagina se modifico y no se ha pasado al disco se salva en el disco 
    if (table[indexEmpty].flags & FLAG_MODY)
        core.devices.writeDevice(table[indexEmpty].did, table[indexEmpty].idResorce, 1, (char*) table[indexEmpty].ptrMem);
    if (abuffer) {
        core.devices.readDevice(adevice, ablock, 1, (char*) table[indexEmpty].ptrMem);
        core.memory.memcpy(abuffer, (char*) table[indexEmpty].ptrMem, 512);
    }
    table[indexEmpty].did = adevice;
    table[indexEmpty].idResorce = ablock;
    table[indexEmpty].flags = FLAG_USED;
    table[indexEmpty].old = 1; //edad minima
    return indexEmpty;
}

int CcacheDisk::writeSector(u32 adevice, u32 ablock, char *abuffer) {
    for (int i = 0; i < MAX_ENTRY; i++) {
        if (table[i].flags != 0) {
            if (table[i].did == adevice && table[i].idResorce == ablock) {
                core.memory.memcpy((char*) table[i].ptrMem, abuffer, 512);
                table[i].old = 1; //Edad minima
                table[i].flags |= FLAG_MODY; //Se indica que se modifico
                return ablock;
            }
        }
    }
    //Si no esta en cache Se carga en cache sin leer
    int indexEmpty = readSector(adevice, ablock, 0);
    core.memory.memcpy((char*) table[indexEmpty].ptrMem, abuffer, 512);
    table[indexEmpty].flags |= FLAG_MODY;
}

void CcacheDisk::flush() {
    for (int i = 0; i < MAX_ENTRY; i++) {
        if (table[i].flags != 0) {
            if (table[i].flags & FLAG_MODY) {
                char *ptr = (char*) table[i].ptrMem;
                core.devices.writeDevice(table[i].did, table[i].idResorce, 1, ptr);
                table[i].flags ^= FLAG_MODY;
            }
        }
    }
}

int CcacheDisk::readSector(u32 adevice, u32 ablock, u32 asectorCount, char *abuffer) {
    char *buffer = abuffer;
    u32 block = ablock;
    for (int i = 0; i < asectorCount; i++) {
        readSector(adevice, block, buffer);
        buffer += 512;
        block++;
    }
}

int CcacheDisk::writeSector(u32 adevice, u32 ablock, u32 asectorCount, char *abuffer) {
    char *buffer = abuffer;
    u32 block = ablock;
    for (int i = 0; i < asectorCount; i++) {
        writeSector(adevice, block, buffer);
        buffer += 512;
        block++;
    }
}