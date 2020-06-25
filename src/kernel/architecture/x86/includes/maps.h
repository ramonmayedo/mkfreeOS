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

#ifndef MAPS_H
#define MAPS_H

#include "../../../defines/types.h"

//----Mapeo de descriptores para modo protegido---------------------------------
/*       Formato del descriptor Tama?o 8 Bytes

7 | Base (B31 - B24)   | G D O AV Limite (L19 - L16) | 6
5 | Derechos de acceso | (Base B23 - B16)            | 4
3 |              Base (B15 - B0)                     | 2
1 |              Limite (L15 - L0)                   | 0
*/

struct SgdtDescriptor{
	u16 limit0_15;
	u16 base0_15;
	u8  base16_23;
	u8  dacces;
	u8  limit16_19:4;
	u8  option:4;
	u8  base24_31;
}__attribute__ ((packed));

/* Registro de la tabla de descriptores globales "Contiene direccion base y limite" */
struct Sgdtr{
        u16 limit;
       	u32 base;	
} __attribute__ ((packed));

/* Descriptores de interrupciones " Descriptor de Compuerta" 
*  |            Desplazamiento(31-16)                      |6
*  |P | DPL | TIPO | O | O O |Contador de Palabras (C4-C0) |4
 * |                    Selector                           |2
 * |            Desplazamiento(15-00)                      |0
*/
struct SidtDescriptor {
	u16 offset0_15;
	u16 selector;
	u16 type;
	u16 offset16_31;
}__attribute__ ((packed));

/*   Registros de la tabla de interrupciones IDTR*/
struct Sidtr {
	u16 limit;
	u32 base;
}__attribute__ ((packed));

/*          Segmento de estado de tarea 64 Bytes */
struct Stss {
	u16 ret_task,ret_task_unused;
	u32 esp0;
	u16 ss0, ss0_unused;
	u32 esp1;
	u16 ss1, ss1_unused;
	u32 esp2;
	u16 ss2, ss2_unused;
	u32 cr3;
	u32 eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	u16 es, es_unused;
	u16 cs, cs_unused;
	u16 ss, ss_unused;
	u16 ds, ds_unused;
	u16 fs, fs_unused;
	u16 gs, gs_unused;
	u16 ldt_selector, ldt_sel_unused;
	u16 debug_flag, io_map;
}__attribute__ ((packed));

//---------------------Mapeo de Dispositivos------------------------------------
struct SdevicePort {
    u32 regBase;
    u32 regControl;
} __attribute__((packed));

//--------------------Mapeo Puertos PIC 8259------------------------------------
struct SpicMap{
    SdevicePort master;
    SdevicePort slave;
}__attribute__ ((packed));
//--------------------Mapeo Teclado---------------------------------------------
struct SkeyMap{
    SdevicePort port;
    u16 keyState; 
    u8 *scanScode;
    u16 keyPress;
}__attribute__ ((packed));
//-------------------Mapeo del Mouse--------------------------------------------
struct SmouseMap {
    SdevicePort port;
    int dx;
    int dy;
    int dz;
    int state;
    u8 flags;
    u8 buttons;
} __attribute__((packed));
//--------------------Mapeo Clock 8253 -----------------------------------------
#define MAX_TICK_X_SECONDS 1000
struct SclockMap{
    SdevicePort port;
    u32 hour;
    u32 minute;
    u32 second;
    u32 msecond;                        //conteo de los microsegundos
    u32 year;
    u32 month;
    u32 day;
    u32 weekDay;
}__attribute__ ((packed));
//---------------Mapeo para manejar los discos duros----------------------------
struct SdiskIdeMap{
    SdevicePort chanel0;
    SdevicePort chanel1;
} __attribute__((packed));
//-------------------Mapeo de Paginacion----------------------------------------
#define	PAGE_SIZE             0x00001000  //0x00001000 4K Tamaño de una pagina
#define	RAM_MAX_SIZE          0x10000000  //0x10000000 256 MByte Maximo tamaño de RAM
#define	RAM_MAX_PAGE          RAM_MAX_SIZE/PAGE_SIZE  //Maxima cantidad de paginas 0x00010000(65535)

#define KERNEL_PAGE_DIR       0x003FF000  //Directorio de Paginas 4KB 1024 entradas X 4 byte de tamaño
#define KERNEL_PAGE1          0x00400000  //Inicio del marco de paginas del kernel offset 0x00400000 tamaño 4MByte
#define KERNEL_PAGE1_END      0x00800000  //Fin del marco de paginas del kernel offset 0x00800000
#define KERNEL_END            0x00800000  //Fin del espacio de memoria del kerne offset 0x00800000 8MByte
#define KERNEL_PAGE_HEAP      0x00800000  //Memoria Virtual para reservar paginas
#define KERNEL_PAGE_HEP_END   0x08000000  //Fin 128 Mbyte tamaño de 120 MByte
#define KERN_HEAP             0x08000000  //Memoria Virtual de malloc offset 0x08000000 128 Mbyte
#define KERN_HEAP_END         0x40000000  //Termina en 1 GByte offset 0x40000000 tamaño 896 MByte
#define USER_PAGE             0x40000000  //Memoria de usuario offset 0x40000000 1GByte
#define USER_PAGE_SHARED      0x80000000  //Memoria compartida para aplicaciones
#define USER_PAGE_SHARED_END  0xA0000000  //Termina la memoria compartida
#define USER_PAGE_END         0xA0000000  //Termina en  0xA0000000 tamaño 1,5GByte
#define USER_STACK            0xC0000000  //0,5 GByte para la pila de usuario
#define VIRTUAL_PHEAP         (KERNEL_PAGE_HEP_END -  KERNEL_PAGE_HEAP)/PAGE_SIZE //

struct SmemoryPagination{
    u32 ramMaxSize;                  //Maximo tamaño que soprta la RAM (No tamaño real) por defecto 0x10000000 256MByte
    u32 pageSize;                    //Tamaño que va a tener la pagina por defecto 0x00001000 4KByte
    u32 ramMaxPage;                  //Cantidad maxima de paginas por defecto  0x00010000 (65535)
    u32 ramPhisic;                   //Cantidad real de ram disponible en el sistema
    u32 kernelMaxPageVirtual;        //Cantidad maxima de paginas virtuales
    u32 userMaxPageVirtual;          //
    u32 directoryPageKernel;         //Directorio de paginas del kernel offeset 0x003FF000 
    u32 kernelPage1;                 //Posicion del la tabla de paginas del kernel offset 0x00400000 4MB
    u32 kernelPage1End;              //Posicion final de la tabla de paginas del kernel offset 0x00800000 8MB
    u32 kernelEnd;                   //Fin del espacio de memoria del kernel offset 0x00800000 8MB
    u32 kernelPageHeap;              //Memoria Virtual para reservar paginas
    u32 kernelPageHeapEnd;           //Fin 16 Mbyte tamaño de 8 Mbyte
    u32 kernelHeap;                  //Comienzo de la Memoria Virtual malloc offset 0x01000000 16MB  Tamaño 16MB 
    u32 kernelHeapEnd;               //Fin de la Memoria virtual offset 0x02000000 32 MB
    u32 kernelHeapPtr;               //Puntero actual de la memoria tope de la memoria virtual     
    u32 userPageHeap;                //Comienzo de la memoria para usurios offset 0x02000000 32 MB tamaño 32MB
    u32 userPageHeapEnd;             //Fin de la Memoria de Usuario offset 0x04000000 64MB
    u32 userStack;
    u8  bitMapPages[RAM_MAX_PAGE/8];          //Mapa de bit para saber el estado de las paginas
    u8  bitMapPagesKernel[VIRTUAL_PHEAP/8];   //Mapa de bit para para las paginas virtuales
    u32 bitMapPageSize;                       //Tamaño del bitmap 
}__attribute__ ((packed));

//------------------Mapeo para Memoria Virtual----------------------------------
struct SvirtualMemory{
    char *starMemory;
    char *endMemory;   
}__attribute__ ((packed));

//-----------------Mapeo para memoria contigua no asignada---------------------- 
struct SmallocHeader {
    u8  pgsize:2;
    u8  used :1;
    u8  option:5;
    u32 size;
} __attribute__ ((packed));
#endif