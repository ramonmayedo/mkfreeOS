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

#include "includes/hvirtualmemory.h"
#include "../../vmemory/memorybitmap.h"
#include "../../vmemory/memoryzone.h"
#include "../../vmemory/memoryregion.h"
#include "../../uses/list.h"

class CvirtualMemory {
public:
    CvirtualMemory();
    void initialize();
    u8 * getPhisicalPage();
    int addDirectoryPagesKernel(u8 *avirtualAddress, u8 *aphisicalAddress, u32 aflags);
    u8* getPagesHeapToKernel(u32 *phisicalPage);
    u8* getPagesHeapToKernel();
    int releasePagesHeapToKernel(u8 *avirtualAddress);
    u32 *createPageDirectoryUser(u32 *phisicalAddress);
    int insertPageToPageDirectoryUser(Cprocess *process, u8 *avirtualAddress, u8 *aphisicalAdress);
    void deleteDirectoryPagesUser(u32 *apagDirectory);
    void addRegion(CmemoryRegion *region);
    int command(int acommand, int parameter1, int parameter2,int parameter3);
private:
    u32 *dirPagesKernel;
    CmemoryZone *kernelPageHeap;   //Paginas del monton del kernel
    CmemoryZone *sharedMemoryPage;     //Memoria Compartida
    CmemoryBitmap phisicalBitmap;  //Bitmap de las paginas fisicas
    Clist regions;   //Regiones de Memoria y Paginas Compartidas
    int idCountRegion;                //Para llave de la memoria compartida
    void initDirectoryPages();
    int directoryPagesKernel(u8 *avirtualAddress, u8 *aphisicalAddress, u32 aflags, int acommand);
};

#endif