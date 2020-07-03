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

#ifndef DISKPARTITION_H
#define DISKPARTITION_H
#include "includes/disk.h"
#include "../uses/list.h"

#define DISKP_NOT_PARTITION 0x1
#define DISKP_MBR           0x1
#define DISKP_CDROM         0x2

struct SdescriptorPartition {
    u8 typePartition;
    u32 startLBA;
    u32 sizePartiton;
    int device;
} __attribute__((packed));

class CdiskPartition {
public:
    CdiskPartition();
    int initialize();
    int getPartition(int aindexPartition, int adevice, char *buffer);
    int getFileSystem(int adevice);
    int mountFileSystem(int adevice);
private:
    void addPartition(int adevice);
    Clist *partitions;
    Spartition partition; //Particion sobre la cual se trabaja para usarla en todas las funciones
};

#endif

