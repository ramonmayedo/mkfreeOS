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

#include "pic8259.h"
#include "x86.h"

extern Smaps maps;
extern Sx86 x86;

Cpic8259::Cpic8259() {
    
}

void Cpic8259::initialize() {
    	/* Initialization of ICW1 */
        /* | A0 |  A1 - A3 (Solo 8086 No. Vector) | 1 | LTM | ADI | SINGL | ICW4  |
         * Master ICW1 palabra 0x11
         * LTM = 0 Modo de disparo por flanco
         * ADI = 0 LLmada de intervalo de direcion intervalos de 8
         * SINGL = 0 Modo cascada
         * ICW4 = 1 Necesita ICW4
         */
	x86.port.outb(PC_MASTER_BASE_REGISTER, 0x11); //Master ICW1
        /*Segundo 8259A en cascada igual palabra de control que el anterior*/
	x86.port.outb(PC_SLAVE_BASE_REGISTER, 0x11);

	// Initialization of ICW2
        /*Selecciona el numero de vector en las entradas de solicitud de interrupcion
         * Palabra 0x20 
        /*| A0 | A15 / T7 | A14 / T6 | A13 / T5 | A12 / T4 | A11 / T3 | A10 | A9 | A8 |
         * Master 1 vector empieza en 0x20
         */
	x86.port.outb(PC_MASTER_CONTROL_REGISTER, 0x20);	// start vector = 32 
        /*Master 2 vector empieza en 0x70 */
	x86.port.outb(PC_SLAVE_CONTROL_REGISTER, 0x28);	// start vector = 7-15 

	// Initialization of ICW3 Indica donde esta conectado el escalvo con el amo 
        /*ICW 3 palabra 0x04
         * | A0 | 0 | 0 | 0 | 0 | 0 | ID0 | ID1 | ID3 |
         * Esclvo por IRQ 2  0100
         */
	x86.port.outb(PC_MASTER_CONTROL_REGISTER, 0x04);
        
        /*Esclvo envia por la linea IRQ2 */
	x86.port.outb(PC_SLAVE_CONTROL_REGISTER, 0x02);

	// Initialization of ICW4
        /*Se programa para trabajar con 8086-80486
         * | A0 | 0 | 0 | 0 | SFMM | BUF | M / S | AEOI | uPM |
         * SFMM = 0 Modo especial totalmente anidado
         * AEOI =0  EOI Normal 
         * uPM = 1  Modo 8086/8088
         */
	x86.port.outb(PC_MASTER_CONTROL_REGISTER, 0x01);
        /*igual que el anterior*/
	x86.port.outb(PC_SLAVE_CONTROL_REGISTER, 0x01);

        /*Palabra de comando IW1 Inicializa y lee mascara de interrupcion
         * | A0 | M7 |  M6 | M5 | M4 | M3 | M2 | M1 | M0 |
         * Se debe programar luego de la inicializacion ya que se desconoce 
         */
	x86.port.outb(PC_MASTER_CONTROL_REGISTER, 0x0);
        /*igual que el anterior*/
	x86.port.outb(PC_SLAVE_CONTROL_REGISTER, 0x0);
       
}

void Cpic8259::setICW(u32 aicw, u32 aport) {
    x86.port.outb(aport, aicw);
}

void Cpic8259::eoi() {
    x86.port.outb(PC_MASTER_BASE_REGISTER, 0x20);
    x86.port.outb(PC_SLAVE_BASE_REGISTER, 0x20);
}

void Cpic8259::diseabled() {
    x86.port.outb(PC_MASTER_CONTROL_REGISTER, 0xFF);
    x86.port.outb(PC_SLAVE_CONTROL_REGISTER, 0xFF);
}
