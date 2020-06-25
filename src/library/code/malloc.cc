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

#include "../includes/stdlib.h"
#include "../includes/unistd.h"
#include "../includes/memory.h"
#include "../includes/stdio.h"
struct SmallocHeader {
    u8 pgsize : 2;
    u8 used : 1;
    u8 option : 5;
    u32 size;
} __attribute__((packed));

 extern u8 *b_heap;
 extern u8 *e_heap;
 extern u8 context;
 
 u8 *b_heap = 0;
 u8 *e_heap = 0;
 u8 context = 0;
 
void* malloc(u32 size) {

    SmallocHeader *blockMemory;
    u32 mallocSize = size + sizeof (SmallocHeader);
    if (mallocSize < 16) mallocSize = 16;

    if (b_heap == 0) {                         //se inicializa el heap
        b_heap = (u8*) sbrk(mallocSize);       //comienzo del heap siempre despues del fin de bss
        e_heap = b_heap + mallocSize;
        blockMemory = (SmallocHeader*) b_heap;
        blockMemory->used = 0;
        blockMemory->size = mallocSize;
       
    } else {
        blockMemory = (SmallocHeader*) b_heap;
        while (blockMemory->used || blockMemory->size < mallocSize) {
            if (blockMemory->size == 0) return (void*) - 1;
            blockMemory = (SmallocHeader*) ((u8*) blockMemory + blockMemory->size);//proximo bloque
            if (blockMemory == (SmallocHeader*) e_heap) {     //si es el ultimo bloque
                blockMemory = (SmallocHeader*) e_heap;        //se actualiza el heap
                blockMemory->used = 0;
                blockMemory->size = mallocSize;
                e_heap += mallocSize;                         //se actualiza el fin del heap
            } else if (blockMemory > (SmallocHeader*) e_heap) return (void*) - 1; //si esta por encima del fin bugs
        }
        
    }

    u32 restSize = blockMemory->size - mallocSize; //cuanto es mayor el nuevo bloque
    if (restSize < 16)
        blockMemory->used = 1; //se marca comu usado
    else {
        SmallocHeader* partBlockMemory = (SmallocHeader*) ((u8*) blockMemory + mallocSize);
        partBlockMemory->size = restSize;
        partBlockMemory->used = 0;
        blockMemory->size = mallocSize;
        blockMemory->used = 1;
    }
    return ((u8*) blockMemory + sizeof (SmallocHeader));
}

void free(void *amemory) {
    u8* memoryPtr = (u8*) amemory;
    if (memoryPtr == 0) return;
    if ((memoryPtr - sizeof (SmallocHeader)) < b_heap) return;
    if (memoryPtr > e_heap) return;
    SmallocHeader *blockMemory = (SmallocHeader*) (memoryPtr - sizeof (SmallocHeader));
    if (blockMemory->size == 0) return;
    blockMemory->used = 0;

    SmallocHeader *partBlockMemory;
    partBlockMemory = (SmallocHeader *) ((u8*) blockMemory + blockMemory->size);
    while (partBlockMemory < (SmallocHeader *) e_heap && partBlockMemory->used == 0) {
        blockMemory->size += partBlockMemory->size;
        partBlockMemory = (SmallocHeader *) ((u8*) blockMemory + blockMemory->size);
    }
    return;
}

void* realloc(void* amemory, u32 asize) {
    if (amemory == 0) {
        return malloc(asize);
    } else if (asize == 0)
        return amemory;

    SmallocHeader *blockMemory = (SmallocHeader*) ((u8*)amemory - sizeof (SmallocHeader));
    if (blockMemory->size == 0) return amemory;
    
    u32 size = blockMemory->size;
    void *newptr = malloc(size + asize);

    memcpy(newptr, amemory, size);
    free(amemory);
    return newptr;
}

void* calloc(u32 size1, u32 size2) {
    return malloc(size1 * size2);
}
