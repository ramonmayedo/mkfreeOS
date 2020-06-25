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

#ifndef VIRTUALMEMORY_H
#define VIRTUALMEMORY_H

#include "../../defines/types.h"
#include "../../core/virtualmemoryzone.h"

#define PG_PRESENT   0x00000001
#define PG_WRITE     0x00000002
#define PG_USER      0x00000004
#define PG_4MB       0x00000080

#define WRITE_PAGE 0x1
#define RELEASE_PAGE 0x2

class CvirtualMemory {
public:
    CvirtualMemory();
    void initialize();
    u8 * getPageFrameToBitmap();
    int directoryPages(u32 *apagDirectory, u8 *avirtualAddress, u8 *aphisicalAddress, u32 aflags, int acommand);
    int addDirectoryPagesKernel(u8 *avirtualAddress, u8 *aphisicalAddress, u32 aflags);
    u8* getPagesVirtualKernel(u32 *phisicalPage);
    u8* getPagesVirtualKernel();
    int releasePageVirtualKernel(u8 *avirtualAddress);
    u32 *createPageDirectory(u32 *phisicalAddress);
    int createPageToDirectory(u32 *apagDirectory, u32 *apagDirectoryVirtual, u8 *avirtualAddress, u8 *aphisicalAdress);
    void deleteDirectoryPages(u32 *apagDirectory);

protected:
    void initDirectoryPages();
    void initBitapMemoryPages();
    void initMemoryPagesVirtual();

private:
    u32 *pagDirectory;
    u8 *bitMapPages;
    CvirtualMemoryZone *pagesKernel;
};

#endif