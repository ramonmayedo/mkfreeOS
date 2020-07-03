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

#include "malloc.h"
#include "x86.h"

extern Smaps maps;
extern Sx86 x86;
extern Score core;

Cmalloc::Cmalloc() {

}

void * Cmalloc::malloc(u32 asize) {
    u32 realSize;
    SmallocHeader *blockMemory;
    if (asize == 0) return 0;

    realSize = sizeof (SmallocHeader) + asize;
    if (realSize < 16) realSize = 16;
    blockMemory = (SmallocHeader*) maps.memoryPagination.kernelHeap;

    while (blockMemory->used || realSize > blockMemory->size) {

        if (blockMemory->size == 0)
            x86.architecture.kernelStopScreen(ARCHX86_MEMORY_CORRUPT);

        blockMemory = (SmallocHeader*) ((u8*) blockMemory + blockMemory->size);

        if (blockMemory == (SmallocHeader*) maps.memoryPagination.kernelHeapPtr) {
            u32 sizeRes = asize / maps.memoryPagination.pageSize + 1;
            int error = (int) bsrk(sizeRes);
            if (error == -1) {
                x86.ioScreen.printf("KERNEL malloc Error, No hay Memoria para asignar al Kernel /n");
                return 0;
            }
        } else if (blockMemory > (SmallocHeader*) maps.memoryPagination.kernelHeapEnd) {
            x86.ioScreen.printf("KERNEL malloc Error, Ha excedido el limite de memoria del Kernel /n");
            return 0;
        }
    }
    u32 restSize = blockMemory->size - realSize;
    if (restSize < 16)
        blockMemory->used = 1;
    else {
        SmallocHeader* partBlockMemory = (SmallocHeader*) ((u8*) blockMemory + realSize);
        partBlockMemory->size = restSize;
        partBlockMemory->used = 0;
        blockMemory->size = realSize;
        blockMemory->used = 1;
    }
    return ((u8*) blockMemory + sizeof (SmallocHeader));
}

void *Cmalloc::bsrk(u32 anpages) {
    SmallocHeader *blockMemory;
    u8 *newAddressMemoryPhisics;
    u32 newTopMemoryVirtual = maps.memoryPagination.kernelHeapPtr + (anpages * maps.memoryPagination.pageSize);

    if (newTopMemoryVirtual > maps.memoryPagination.kernelHeapEnd) {
        x86.ioScreen.printf("KERNEL kbsrk Error se ha excedido la memoria virtual para el kernel (%i)  /n", maps.memoryPagination.kernelHeapEnd);
        return (char*) - 1;
    }

    blockMemory = (SmallocHeader*) maps.memoryPagination.kernelHeapPtr;

    for (u32 i = 0; i < anpages; i++) {
        newAddressMemoryPhisics = x86.virtualMemory.getPageFrameToBitmap(); //Obtiene direccion fisica libre
        if ((int) newAddressMemoryPhisics == -1) {
            x86.ioScreen.printf("KERNEL kbsrk Error no hay paginas disponibles para asignar al kernel /n");
            return (char*) - 1;
        }
        x86.virtualMemory.addDirectoryPagesKernel((u8*) maps.memoryPagination.kernelHeapPtr, newAddressMemoryPhisics, 0);
        maps.memoryPagination.kernelHeapPtr += maps.memoryPagination.pageSize;
    }

    blockMemory->size = anpages * maps.memoryPagination.pageSize;
    blockMemory->used = 0x0; //si uso
    return blockMemory;
}

void Cmalloc::free(void *amemory) {
    u8* memoryPtr = (u8*) amemory;
    if (memoryPtr == 0) return;

    if (((u32) memoryPtr - sizeof (SmallocHeader)) < maps.memoryPagination.kernelHeap) {
        x86.ioScreen.printf("KERNEL free Este Elemeto esta por debajo del Heap del Kernel (%i) /n", maps.memoryPagination.kernelHeap);
        return;
    }

    if ((u32) memoryPtr > maps.memoryPagination.kernelHeapEnd) {
        x86.ioScreen.printf("KERNEL free Este Elemeto esta por encima del Heap del Kernel (%i) /n", maps.memoryPagination.kernelHeapEnd);
        return;
    }

    SmallocHeader *blockMemory = (SmallocHeader*) (memoryPtr - sizeof (SmallocHeader));

    if (blockMemory->size == 0)
        x86.architecture.kernelStopScreen(ARCHX86_MEMORY_CORRUPT);

    blockMemory->used = 0;

    SmallocHeader *partBlockMemory;
    partBlockMemory = (SmallocHeader *) ((u8*) blockMemory + blockMemory->size);
    while (partBlockMemory < (SmallocHeader *) maps.memoryPagination.kernelHeapPtr && partBlockMemory->used == 0) {
        blockMemory->size += partBlockMemory->size;
        partBlockMemory = (SmallocHeader *) ((u8*) blockMemory + blockMemory->size);
    }
    return;
}