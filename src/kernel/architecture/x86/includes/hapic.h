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

#ifndef HAPIC_H
#define HAPIC_H

//000h - 010h  Reservado   
#define REG_LAPIC_ID  0x20  //Registro LAPIC ID | RW
#define REG_LAPIC_VER 0x30  //Registro de versión LAPIC  | R 
//040h - 070h  Reservado   
#define REG_LAPIC_TPR 0x80  //Registro de prioridad de tareas (TPR)  | RW
#define REG_LAPIC_APR 0x90  //Registro de prioridad de arbitraje (APR)  | RW
#define REG_LAPIC_PPR 0xA0  //Registro de prioridad del procesador (PPR) R
#define REG_LAPIC_EOI 0xB0  //Registro EOI  | W
#define REG_LAPIC_RRD 0xC0  //Registro de lectura remota (RRD)  | R
#define REG_LAPIC_LDR 0xD0  //Registro de destino lógico  | RW  
#define REG_LAPIC_DFR 0xE0  //Registro de formato de destino  | RW
#define REG_LAPIC_VIR 0xF0  //Registro de vectores de interrupción espuria  | RW  0x0F
#define REG_LAPIC_ISR 0x100 //0x100 - 0x170  Registro en servicio (ISR)  | R  
#define REG_LAPIC_TMR 0x180 //0x180 - 0x1F0  Registro de modo de disparo (TMR)  | R
#define REG_LAPIC_TIR 0x200 //0x200 - 0x270  Registro de solicitud de interrupción (TIR) | R  
#define REG_LAPIC_ESR 0x280 //Registro de estado de error  | R  
//290h - 2E0h  Reservado   
#define REG_LAPIC_CMCI  0x2F0 //Registro de interrupción de verificación de máquina corregida LVT (CMCI)  | RW
#define REG_LAPIC_ICRL  0x300 //0x300 Registro de comando de interrupción (ICR) LOW | RW
#define REG_LAPIC_ICRH  0x310 //0x310 Registro de comando de interrupción (ICR) HIGHT | RW
#define REG_LAPIC_LVT   0x320 //Registro de temporizador LVT  | RW   0x320
#define REG_LAPIC_SLVT  0x330 //Registro de sensor térmico LVT  | RW
#define REG_LAPIC_CLVT  0x340 //Registro de contadores de monitoreo de rendimiento LVT | RW
#define REG_LAPIC_LINT0 0x350 // LVT LINT0 Registro  | RW
#define REG_LAPIC_LINT1 0x360 // LVT LINT1 Registro  | RW
#define REG_LAPIC_ELVT  0x370 // Registro de errores de LVT  | RW
#define REG_LAPIC_RRI   0x380  //Registro de recuento inicial (para temporizador) | RW  
#define REG_LAPIC_RRA   0x390  //Registro de recuento actual (para temporizador)  | R
//3A0h - 3D0h  Reservado   
#define REG_LAPIC_DRC 0x3E0 //Divide el registro de configuración (para el temporizador) | RW
//3F0h  Reservado  

#define ENABLE     0x00000100
#define INIT       0x00000500
#define STARTUP    0x00000600
#define DELIVS     0x00001000
#define ASSERT     0x00004000
#define DEASSERT   0x00000000
#define LEVEL      0x00008000
#define BCAST      0x00080000
#define BUSY       0x00001000
#define FIXED      0x00000000

#define REG_ID     0x00  // Register index: ID
#define REG_VER    0x01  // Register index: version
#define REG_TABLE  0x10  // Redirection table base

// CPUs can serve that interrupt.
#define INT_DISABLED   0x00010000  // Interrupt disabled
#define INT_LEVEL      0x00008000  // Level-triggered (vs edge-)
#define INT_ACTIVELOW  0x00002000  // Active low (vs high)
#define INT_LOGICAL    0x00000800  // Destination is CPU id (vs APIC ID)

#define APIC_REG_DF       0xE0
#define APIC_REG_LD       0xD0
#define IRQ_APIC_SPURIOUS 0x1F
#define ID                0x20 
#define APIC_BASE_MSR     0x1B

#define CMOS_PORT    0x70
#define CMOS_RETURN  0x71

struct SptrIoApic {
    u32 reg;
    u32 pad[3];
    u32 data;
} __attribute__((packed));

#endif

