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

#ifndef HMODEPROTECTED_H
#define HMODEPROTECTED_H
#include "../../../defines/types.h"

//Area para definiciones gloables
#define MP_GDTSIZE  0x10
#define MP_IDTSIZE  0xFF
#define MP_INTGATE  0x8E00 //Tipo Compuerta de interrupcion del 80386 
#define MP_TRAPGATE 0xEF00 //Compuerta de trampa del 80386

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


#endif

