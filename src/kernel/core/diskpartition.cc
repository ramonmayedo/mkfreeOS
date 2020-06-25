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

#include "diskpartition.h"
#include "filesystem/fat.h"
#include "filesystem/bfs.h"
#include "../architecture/x86/x86.h"

extern Score core;
extern Sx86 x86;

CdiskPartition::CdiskPartition() {

}

int CdiskPartition::getPartition(int aindexPartition, int adevice) {
    char *buffer = new char[512];
    core.cacheDisk.readSector(adevice, 0, 1, buffer);
    Spartition *ptrPartiton;
    ptrPartiton = (Spartition*) (buffer + PARTITIONBASE + aindexPartition * 16);
    partition[aindexPartition] = *ptrPartiton;
    delete(buffer);
    return 0;
}

int CdiskPartition::getFileSystem(int adevice, int aindexPartition) {
    char *buffer = new char[512]; //Se reservan 512 byte para almacenar el bloque de disco
    if (partition[aindexPartition].typePartition == FAT_32) {
        core.cacheDisk.readSector(adevice, partition[aindexPartition].startLBA, 1, buffer); //Se lee el sector 0 de la particion
        SfatFormat32 * fatFormat32 = new SfatFormat32; //Se guarda la estructura de ficheros
        *fatFormat32 = *(SfatFormat32*) buffer;
        core.fileSystem.mountFileSystem(FAT_32, (void*) fatFormat32, adevice, partition[aindexPartition].startLBA); //Se monta el medio en el sistema
    }else if (partition[aindexPartition].typePartition == BFS){
        core.cacheDisk.readSector(adevice, partition[aindexPartition].startLBA, 1, buffer);
        SblockHead * blockHead = new SblockHead;
        blockHead = (SblockHead*)buffer;
        core.fileSystem.mountFileSystem(BFS, (void*) blockHead, adevice, partition[aindexPartition].startLBA);
    }else{
        x86.ioScreen.printf("device %i tipo %i No se reconoce el sistem de Archivo /n",adevice,partition[aindexPartition].typePartition);
    }
    delete(buffer);
    return 0;
}

int CdiskPartition::mountFileSystem(int adevice) {
    for (int i = 0; i < 1; i++) {
        getPartition(i, adevice);
        getFileSystem(adevice, i);
    }
    return 0;
}