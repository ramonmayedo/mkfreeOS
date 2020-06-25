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

#include "virtualmemoryzone.h"
#include "../architecture/x86/x86.h"

extern Smaps maps;
extern Sx86 x86;
extern Score core;

CvirtualMemoryZone::CvirtualMemoryZone(u8 *abitmap, u32 avAddressStart, u32 avAddressEnd, u32 apageSize, u32 *adirectoryPage) {
    bitmap = abitmap;
    vAddressStart = avAddressStart;
    vAddressEnd = avAddressEnd;
    pageSize = apageSize;
    directoryPage = adirectoryPage;
    size = (vAddressEnd - vAddressStart) / (pageSize * 8);
    for (u32 i = 0; i < size; i++)
        bitmap[i] = 0x0;
    bit = 0;
    index = 0;
}

u8* CvirtualMemoryZone::getPageVirtualToBitmap(u32 *phisicalPage) {
    u8* virtualAdress, *phisicalAddress;
    for (u32 i = 0; i < size; i++) {
        if (bitmap[i] != 0xFF) {
            for (u8 j = 0; j < 8; j++) {
                if ((bitmap[i] & (1 << j)) == 0) {
                    bitmap[i] |= (1 << j);
                    virtualAdress = (u8*) (((8 * i) + j) * pageSize + vAddressStart);
                    phisicalAddress = x86.virtualMemory.getPageFrameToBitmap();
                    *phisicalPage = (u32) phisicalAddress;
                    x86.virtualMemory.directoryPages(directoryPage, virtualAdress, phisicalAddress, 0, WRITE_PAGE);
                    core.memory.memset((char*) virtualAdress, 0x0, maps.memoryPagination.pageSize);
                    return (u8*) virtualAdress;
                }
            }
        }
    }
    x86.ioScreen.printf("getPageVirtualToBitmap() Error no hay memoria virtual para asignar /n");
    return (u8*) - 1;
}

int CvirtualMemoryZone::releasePageVirtual(u8 *avirtualAddress) {
    if ((u32) avirtualAddress > vAddressEnd) {
        x86.ioScreen.printf("releasePageVirtual() Esta fuera de la memoria virtual /n");
        return -1;
    }
    index = ((u32) avirtualAddress - vAddressStart) / pageSize;
    bit = index % 8;
    bitmap[index] ^= (1 << bit);
    return 0;
}