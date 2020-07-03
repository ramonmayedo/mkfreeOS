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
#include "filesystem/iso9660.h"
#include "../architecture/x86/x86.h"

extern Score core;
extern Sx86 x86;

CdiskPartition::CdiskPartition() {

}

int CdiskPartition::initialize() {
    partitions = new Clist;
}

int CdiskPartition::getPartition(int aindexPartition, int adevice, char *buffer) {
    int typeDevice = ((Sdevice*) core.devices.getDevice(adevice))->type;
    switch (typeDevice) {
        case DEV_ATA_PATA://Si es unidad de disco MBR
        {
            Spartition *ptrPartiton = (Spartition*) (buffer + PARTITIONBASE + aindexPartition * 16);
            if (ptrPartiton->sizePartiton == 0) return DISKP_NOT_PARTITION;
            partition.startLBA = ptrPartiton->startLBA;
            partition.sizePartiton = ptrPartiton->sizePartiton;
            partition.typePartition = ptrPartiton->typePartition;
            addPartition(adevice);
            break;
        }
        case DEV_ATA_PATAPI://Si es unidad de disco  UDF Universal Disc Format
        {
            partition.startLBA = 16;
            partition.typePartition = ISO9660;
            addPartition(adevice);
            break;
        }
        default:
        {
            break;
        }
    }
    return 0;
}

int CdiskPartition::getFileSystem(int adevice) {
    char *buffer = new char[512]; //Se reservan 512 byte para almacenar el bloque de disco
    switch (partition.typePartition) {
        case FAT_32:
        {
            core.cacheDisk.readSector(adevice, partition.startLBA, 1, buffer); //Se lee el sector 0 de la particion
            SfatFormat32 * fatFormat32 = new SfatFormat32; //Se guarda la estructura de ficheros
            *fatFormat32 = *(SfatFormat32*) buffer;
            core.fileSystem.mountFileSystem(FAT_32, (void*) fatFormat32, adevice, partition.startLBA); //Se monta el medio en el sistema
            break;
        }
        case BFS:
        {
            core.cacheDisk.readSector(adevice, partition.startLBA, 1, buffer);
            SblockHead * blockHead = new SblockHead;
            blockHead = (SblockHead*) buffer;
            core.fileSystem.mountFileSystem(BFS, (void*) blockHead, adevice, partition.startLBA);
            break;
        }
        case ISO9660:
        {
            char *bufferS = new char[2048];
            if (core.cacheDisk.readSectorExt(adevice, partition.startLBA, 1, bufferS))
                core.fileSystem.mountFileSystem(ISO9660, (void*) bufferS, adevice, partition.startLBA); //Se monta el medio en el sistema
            else x86.ioScreen.printf("No se pudo leer la unidad de CDROM/n");
            break;
        }
        default:
        {
            x86.ioScreen.printf("device %i tipo %i No se reconoce el sistem de Archivo /n", adevice, partition.typePartition);
            break;
        }
    }
    return 0;
}

int CdiskPartition::mountFileSystem(int adevice) {
    int typeDevice = ((Sdevice*) core.devices.getDevice(adevice))->type;
    char *buffer = new char[512];
    switch (typeDevice) {
        case DEV_ATA_PATA: //Si es unidad de disco
        {
            core.cacheDisk.readSector(adevice, 0, 1, buffer); //Se lee el sector 0
            for (int i = 0; i < 4; i++) {                     //Solo MBR cuatro particiones
                if (!getPartition(i, adevice, buffer))        //Si se encuentra la partición
                    getFileSystem(adevice);                   //Se trata de montar el sistema de archivos
            }
            break;
        }
        case DEV_ATA_PATAPI://Si es unidad de disco
        {
            getPartition(0, adevice, buffer);
            getFileSystem(adevice);
            break;
        }
        default: break;
    }
    delete(buffer);
    return 0;
}

void CdiskPartition::addPartition(int adevice) {
    SdescriptorPartition *newPartition = new SdescriptorPartition();
    newPartition->startLBA = partition.startLBA;
    newPartition->sizePartiton = partition.sizePartiton;
    newPartition->typePartition = partition.typePartition;
    newPartition->device = adevice;
    partitions->add(newPartition);
    x86.ioScreen.printf("device %i  Tipo de Particion %i /n", adevice, partition.typePartition);
}