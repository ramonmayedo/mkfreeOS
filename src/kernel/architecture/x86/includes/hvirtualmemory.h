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

#ifndef HVIRTUALMEMORY_H
#define HVIRTUALMEMORY_H

#include "../../../defines/types.h"

enum commandSharedMemory{
    cmsGetZone = 1, cmsAddZone = 2, cmsSharedZone = 3
};

#define NOT_FLAGS              0x0
#define WM_PG_PRESENT          0x00000001
#define WM_PG_WRITE            0x00000002
#define WM_PG_USER             0x00000004
#define WM_PG_4MB              0x00000080
#define WM_PG_CACHE_DISABLED   0x00000010

#define WM_WRITE_PAGE   0x1
#define WM_RELEASE_PAGE 0x2

//-------------------Mapeo de Paginacion----------------------------------------
#define	VM_PAGE_SIZE             0x00001000  //0x00001000 4K Tamaño de una pagina
#define VM_RAM_MAX_SIZE          0x10000000  //0x10000000 256 MByte Maximo tamaño de RAM
#define VM_RAM_MAX_PAGE          VM_RAM_MAX_SIZE/VM_PAGE_SIZE  //Maxima cantidad de paginas 0x00010000(65535)
#define VM_KERNEL_PAGE_DIR       0x003FF000  //Directorio de Paginas 4KB 1024 entradas X 4 byte de tamaño
#define VM_KERNEL_PAGE1          0x00400000  //Inicio del marco de paginas del kernel offset 0x00400000 tamaño 4MByte
#define VM_KERNEL_PAGE1_END      0x00800000  //Fin del marco de paginas del kernel offset 0x00800000
#define VM_KERNEL_END            0x00800000  //Fin del espacio de memoria del kerne offset 0x00800000 8MByte
#define VM_KERNEL_PAGE_HEAP      0x00800000  //Memoria Virtual para reservar paginas
#define VM_KERNEL_PAGE_HEP_END   0x08000000  //Fin 128 Mbyte tamaño de 120 MByte
#define VM_KERNEL_HEAP           0x08000000  //Memoria Virtual de malloc offset 0x08000000 128 Mbyte
#define VM_KERNEL_HEAP_END       0x40000000  //Termina en 1 GByte offset 0x40000000
#define VM_USER_PAGE             0x40000000  //Memoria de usuario offset 0x40000000 1GByte
#define VM_USER_STACK_END        0x80000000  //Fin de pila de usuario
#define VM_USER_STACK            0xA0000000  //0,5 GByte para la pila de usuario
#define VM_USER_PAGE_END         0xA0000000  //Termina en  0xA0000000 tamaño 1,5 GByte
#define VM_USER_PAGE_SHARED      0xA0001000  //Memoria compartida para aplicaciones
#define VM_USER_PAGE_SHARED_END  0xC0000000  //Termina la memoria compartida
#define VM_VIRTUAL_PHEAP         (VM_KERNEL_PAGE_HEP_END -  VM_KERNEL_PAGE_HEAP)/VM_PAGE_SIZE //
#define VM_VIRTUAL_SHARED        (VM_USER_PAGE_SHARED_END - VM_USER_PAGE_SHARED)/VM_PAGE_SIZE //

struct SmemoryPagination{
    u32 ramMaxSize;                              //Maximo tamaño que soprta la RAM (No tamaño real) por defecto 0x10000000 256MByte
    u32 ramPhisic;                               //Cantidad real de ram disponible en el sistema
    u32 kernelHeapPtr;                           //Puntero actual de la memoria tope de la memoria virtual     
    u8  bitMapPages[VM_RAM_MAX_PAGE/8];          //Mapa de bit para saber el estado de las paginas
    u8  bitMapPagesKernel[VM_VIRTUAL_PHEAP/8];   //Mapa de bit para para las paginas virtuales
    u8  bitMapSharedMemory[VM_VIRTUAL_SHARED/8]; //Mapa de bit para para las paginas virtuales
    u32 bitMapPageSize;                          //Tamaño del bitmap 
}__attribute__ ((packed));


#endif

