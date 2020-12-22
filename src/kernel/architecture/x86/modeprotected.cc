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

#include "modeprotected.h"
#include "x86.h"

/*Se hace referencia a estas variables externas que estan en el _main y son del Kernel*/

extern Score core;
extern Sx86 x86;
extern Smaps maps; 
CmodeProtected::CmodeProtected() {

}
//incializa un descriptor global

void CmodeProtected::initGdtDescriptor(u32 abase, u32 alimit, u8 adacces, u8 aoption, struct SgdtDescriptor *adescriptor) {
    adescriptor->limit0_15 = (alimit & 0xffff);
    adescriptor->base0_15 = (abase & 0xffff);
    adescriptor->base16_23 = (abase & 0xff0000) >> 16;
    adescriptor->dacces = adacces;
    adescriptor->limit16_19 = (alimit & 0xf0000) >> 16;
    adescriptor->option = (aoption & 0xf);
    adescriptor->base24_31 = (abase & 0xff000000) >> 24;
}
//inicializa un descriptor de interrupciones

void CmodeProtected::initIdtDescriptor(u16 aselector, u32 aoffset, u16 atype, struct SidtDescriptor *adescriptor) {
    adescriptor->offset0_15 = (aoffset & 0xffff);
    adescriptor->selector = aselector;
    adescriptor->type = atype;
    adescriptor->offset16_31 = (aoffset & 0xffff0000) >> 16;
}


void CmodeProtected::initGdt(Scpus* cpu) {
    cpu->tss.debug_flag = 0x00;
    cpu->tss.io_map = 0x00;
    cpu->tss.esp0 = cpu->kstack; //Direccion para la pila del kernel en el anillo 0
    cpu->tss.ss0 = 0x18;     //Descriptor de segmento de la pila del kernel
    
    /*   Descriptor nulo siempre index 0  */
    initGdtDescriptor(0x0, 0x0, 0x0, 0x0, &cpu->kgdt[0]); //Index 0 0x0 Null

    /* descriptor codigo index 1  base = 0x0 limite = 0xFFFFF   
     derechos de acceso = 0x9B   | P | DPL | S | E | ED/C | R/W | A |
     P=1 El segmento contiene base y limetes validos
     DPL = 00 nivel de privilegio mas alto
     S=1 Descriptor del segmento de codigo o datos
     E=1 El descriptor describe un segmento de codigo
     C=0 El segmento se esxpande en forma ascendente
     R=1 El segmento se puede leer
     A=1 Se acceso al segmento
     option = 0x0D | G | D | O | AV| parte alta del byte
     G =1 Bit de granalaridad, el valor limite es multiplicado por 4Kb
     D =1 Supone que las instrucciones son 32bits
     AV = 1 El segmento esta disponible
     */
    initGdtDescriptor(0x0, 0xFFFFF, 0x9B, 0x0D, &cpu->kgdt[1]); //Index 1 0x8 KCS

    /* descriptor datos index 2  base = 0x0 limite = 0xFFFFF   
    derechos de acceso = 0x93   | P | DPL | S | E | ED/C | R/W | A |
    P=1 El segmento contiene base y limetes validos
    DPL = 00 nivel de privilegio mas alto
    S=1 Descriptor del segmento de codigo o datos
    E=0 El descriptor describe un segmento de datos
    EDC = 0 El segmento se expande en forma ascedente
    R=1 El segmento se puede leer
    A=1 Se acceso al segmento
    option = 0x0D | G | D | O | AV| parte alta del byte
    G =1 Bit de granalaridad, el valor limite es multiplicado por 4Kb
    D =1 Supone que las instrucciones son 32bits
    AV = 1 El segmento esta disponible
     */
    initGdtDescriptor(0x0, 0xFFFFF, 0x93, 0x0D, &cpu->kgdt[2]); //Index 2 0x10 KDS

    /* descriptor pila index 3  base = 0x0 limite = 0xfff  4k   
    derechos de acceso = 0x97   | P | DPL | S | E | ED/C | R/W | A |
    P=1 El segmento contiene base y limetes validos
    DPL = 00 nivel de privilegio mas alto
    S=1 Descriptor del segmento de codigo o datos
    E=0 El descriptor describe un segmento de datos
    EDC = 1 El segmento se expande en forma descedente
    R=1 El segmento se puede leer
    A=1 Se acceso al segmento
    option = 0x0D | G | D | O | AV| parte alta del byte
    G =1 Bit de granalaridad, el valor limite es multiplicado por 4Kb
    D =1 Supone que las instrucciones son 32bits
    AV = 1 El segmento esta disponible
     */
    initGdtDescriptor(0x0, 0x00000, 0x97, 0x0D, &cpu->kgdt[3]); //Index 3 0x18 KSS

    /* descriptor codigo index 4  base = 0x0 limite = 0xffffffff   
    derechos de acceso = 0xff   | P | DPL | S | E | ED/C | R/W | A |
    P=1 El segmento contiene base y limetes validos
    DPL = 11 nivel de privilegio mas bajo
    S=1 Descriptor del segmento de codigo o datos
    E=1 El descriptor describe un segmento de codigo
    EDC = 1 Respetar el grado de privilegio
    R=1 El segmento se puede leer
    A=1 Se acceso al segmento
    option = 0x0D | G | D | O | AV| parte alta del byte
    G =1 Bit de granalaridad, el valor limite es multiplicado por 4Kb
    D =1 Supone que las instrucciones son 32bits
    AV = 1 El segmento esta disponible
     */
    initGdtDescriptor(0x0, 0xFFFFF, 0xFF, 0x0D, &cpu->kgdt[4]);  //index 4 0x20 Tarea CS
 
    /* descriptor dato index 5  base = 0x0 limite = 0xffffffff   
    derechos de acceso = 0xff   | P | DPL | S | E | ED/C | R/W | A |
    P=1 El segmento contiene base y limetes validos
    DPL = 11 nivel de privilegio mas bajo
    S=1 Descriptor del segmento de codigo o datos
    E=0 El descriptor describe un segmento de datos
    EDC = 0 El segmento se expande en forma ascedente
    R=1 Se pueden scribir los datos
    A=1 Se acceso al segmento
    option = 0x0D | G | D | O | AV| parte alta del byte
    G =1 Bit de granalaridad, el valor limite es multiplicado por 4Kb
    D =1 Supone que las instrucciones son 32bits
    AV = 1 El segmento esta disponible
     */      
    initGdtDescriptor(0x0, 0xFFFFF, 0xF3, 0x0D, &cpu->kgdt[5]);  //index 5 0x28 Tarea DS     
    
    /* descriptor pila index 6  base = 0x0 limite = 0x0   
    derechos de acceso = 0xff   | P | DPL | S | E | ED/C | R/W | A |
    P=1 El segmento contiene base y limetes validos
    DPL = 11 nivel de privilegio mas bajo
    S=1 Descriptor del segmento de codigo o datos
    E=0 El descriptor describe un segmento de datos
    EDC = 1 El segmento se expande en forma descedente
    R=1 Se pueden scribir los datos
    A=1 Se acceso al segmento
    option = 0x0D | G | D | O | AV| parte alta del byte
    G =1 Bit de granalaridad, el valor limite es multiplicado por 4Kb
    D =1 Supone que las instrucciones son 32bits
    AV = 1 El segmento esta disponible
     */ 
    initGdtDescriptor(0x0, 0x0, 0xF7, 0x0D, &cpu->kgdt[6]);   //index 6 0x30 Tarea SS
        
    /* Descriptor codigo index 7  base = tss limite = 0x67 tamaño de un tss 
     * Descriptor de sistema 
     * Derecho de acceso 0xE9
     * P = 1 El segmento contiene base y limetes validos esta presente
     * DPL = 11 Grado de privilegio mas bajo
     * S = 0 Descriptor del sistema
     * Tipo = 1 00 1 TSS 80386 disponible
     */
    initGdtDescriptor((u32)&cpu->tss, 0x67, 0xE9, 0x00, &cpu->kgdt[7]); //index 7 0x38   
  
    cpu->gdtr.limit = 8*8;
    cpu->gdtr.base = (u32)cpu->kgdt; 
    Sgdtr *ptr = &cpu->gdtr;
    
    asm volatile("lgdtl (%0)" : "=r"(ptr));
    asm volatile("movw $0x10, %ax	\n \
    movw %ax, %ds	\n \
    movw %ax, %es	\n \
    movw %ax, %fs	\n \
    movw %ax, %gs	\n \
    ljmp $0x08,$change	\n \
    change: nop		\n");
    asm volatile("movw $0x18, %ax    \n \
    movw %ax, %ss           \n \
    ");

}

extern u32 asm_passint;
extern u32 asm_int_keyboard;
extern u32 asm_int_timer;
extern u32 asm_int_disk0;
extern u32 asm_gp;
extern u32 asm_page_fault;
extern u32 asm_syscall;
extern u32 asm_int_mouse_ps2;

void CmodeProtected::initIdt(void) {

    for (int i = 0; i < MP_IDTSIZE; i++) //De la int 0 a int 0xFF se inicializan iguales a rutina qu no hace nada
        initIdtDescriptor(0x08, (u32) & asm_passint, MP_INTGATE, &kidt[i]); // 

    initIdtDescriptor(0x08, (u32) & asm_passint, MP_INTGATE, &kidt[0]);

    initIdtDescriptor(0x08, (u32) & asm_passint, MP_INTGATE, &kidt[1]);

    initIdtDescriptor(0x08, (u32) & asm_gp, MP_INTGATE, &kidt[11]); //GP

    initIdtDescriptor(0x08, (u32) & asm_gp, MP_INTGATE, &kidt[13]); //GP

    initIdtDescriptor(0x08, (u32) & asm_page_fault, MP_INTGATE, &kidt[14]); //Falla de Pagina

    //Interrupcion de reloj IRQ 0 0x20
    initIdtDescriptor(0x08, (u32) & asm_int_timer, MP_INTGATE, &kidt[0x20]);
    //Interrupcion de teclado IRQ 1 0x21
    initIdtDescriptor(0x08, (u32) & asm_int_keyboard, MP_INTGATE, &kidt[0x21]);
    //Interrupcion al Mause IRQ 12 0x2C
    initIdtDescriptor(0x08, (u32) & asm_int_mouse_ps2, MP_INTGATE, &kidt[0x2C]);    
    //Interrupcion del disk 0 IRQ 14 0x2E
    initIdtDescriptor(0x08, (u32) & asm_int_disk0, MP_INTGATE, &kidt[0x2E]);
    //Interrupcion del disk 1 IRQ 14 0x2E
    initIdtDescriptor(0x08, (u32) & asm_int_disk0, MP_INTGATE, &kidt[0x2F]);
    
    initIdtDescriptor(0x08, (u32) & asm_passint, MP_TRAPGATE, &kidt[48]);
    //Interrupcion 0x80 usada para las llamadas al kernel sysCall
    initIdtDescriptor(0x08, (u32) & asm_syscall, MP_TRAPGATE, &kidt[128]);

    kidtr.limit = MP_IDTSIZE * 8;
    kidtr.base = (u32) kidt;
    Sidtr *ptr = &kidtr;
    asm volatile("lidtl (%0)" : "=r"(ptr));

}

void CmodeProtected::addHandlerInterrupt(u8 index, u32 handler) {
    initIdtDescriptor(0x08, (u32) & handler, MP_INTGATE, &kidt[index]);
}

//Para las CPU que no es la BP

void CmodeProtected::initGDTCpus(Scpus* cpu) {
    core.memory.memcpy((char*) cpu->kgdt, (char*)x86.cpu[0]->kgdt, 8 * 7); //Solo los primero 7 Descriptores son comunes
    initGdtDescriptor((u32) & cpu->tss, 0x67, 0xE9, 0x00, &cpu->kgdt[7]); //index 7 0x38 Tss de esta CPU que es diferente  
    cpu->gdtr.limit = 8 * 8;
    cpu->gdtr.base = (u32) cpu->kgdt;
    cpu->kstack = (u32) x86.virtualMemory.getPagesHeapToKernel() - 16;  //Direccion para la pila del kernel en el anillo 0
    cpu->tss.debug_flag = 0x00;
    cpu->tss.io_map = 0xFFFF;
    cpu->tss.esp0 = cpu->kstack;
    cpu->tss.ss0 = 0x18;                                                 //Descriptor de segmento de la pila del kernel
}