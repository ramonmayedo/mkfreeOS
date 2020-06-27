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

#include "intfunction.h"
#include "x86.h"
#include "../../core/process.h"

extern Score core;
extern Smaps maps;
extern Sx86 x86;

void c_passint() {
    x86.pic8259.reconocer();
}
//Atencion al timer IRQ0

void c_int_timer() {
    u32 *ebp;
    asm("mov %%ebp,%0" : "=m"(ebp));
    Cprocess *process = core.adminProcess.getRun();
    if (process != 0) {
        maps.ticks++;
        maps.clockMap.msecond++;
    }
    if (maps.clockMap.msecond >= MAX_TICK_X_SECONDS) {
        maps.clockMap.msecond = 0;
        core.cacheDisk.flush();
    }
    if (maps.ticks >= 10) {
        maps.ticks = 0;
        
        if (process != 0 && core.adminProcess.getNext() != 0) {
            process->processX86.regX86->gs = ebp[2];
            process->processX86.regX86->fs = ebp[3];
            process->processX86.regX86->es = ebp[4];
            process->processX86.regX86->ds = ebp[5];
            process->processX86.regX86->edi = ebp[6];
            process->processX86.regX86->esi = ebp[7];
            process->processX86.regX86->ebp = ebp[8];
            process->processX86.regX86->ebx = ebp[10];
            process->processX86.regX86->edx = ebp[11];
            process->processX86.regX86->ecx = ebp[12];
            process->processX86.regX86->eax = ebp[13];
            process->processX86.regX86->eip = ebp[14];
            process->processX86.regX86->cs = ebp[15];
            process->processX86.regX86->eflags = ebp[16];
            process->processX86.regX86->esp = ebp[17];
            core.adminProcess.schelude();
        }
    }
    x86.pic8259.reconocer();
}

void c_int_keyboard() {
    core.keyborad.IRQ();
}

void c_int_disk0() {
    // x86.ioScreen.printf("disk/n");
    // while(1);
    x86.pic8259.reconocer();
}

void c_mouse_ps2() {
    core.mousePS2.IRQ();
}

void c_gp() {
    int eip, cs, ebp;
    asm("movl 64(%ebp),%eax");
    asm("movl %%eax,%0" : "=m"(cs));
    asm("movl 60(%ebp),%eax");
    asm("movl %%eax,%0" : "=m"(eip));
    Cprocess * process = core.adminProcess.getRun();
    char string[10];
    x86.ioScreen.clearScreen();
    x86.ioScreen.printf("/n----!!!! Falla General Proteccion GP !!!!----/n");
    core.conversion.IntToHexChar(cs, string, 4);
    x86.ioScreen.printf("         CS = %s : ", string);
    core.conversion.IntToHexChar(eip, string, 8);
    x86.ioScreen.printf("EIP = %s /n", string);
    x86.ioScreen.printf("       ----- Process PID = %i -----/n",process->getpid());
    core.conversion.IntToHexChar(process->processX86.regX86->eip, string, 8);
    x86.ioScreen.printf("EIP = %s /n/n", string);
    core.adminProcess.killProcessRun();
    while(1);
}

void c_page_fault() {
    char string[10];
    u32 dirFalla, val;
    Cprocess *proceso;
    proceso = (Cprocess*) core.adminProcess.getRun();
    asm("mov %cr2, %eax");
    asm("mov %%eax, %0" : "=m"(dirFalla));
    bool error = false;
    if (dirFalla < maps.memoryPagination.userPageHeap || dirFalla > maps.memoryPagination.userPageHeapEnd)
        error = true;
    else {
         
        if (proceso != 0) {
            u32 *directory = proceso->processX86.pageDirectory;
            u32 *directoryVirtual = proceso->processX86.pageDirectoryVirtual;
            x86.virtualMemory.createPageToDirectory(directory, directoryVirtual, (u8*) dirFalla, 0);
            return;
        }
    }
    x86.ioScreen.clearScreen();
    x86.ioScreen.printf("----!!!! Falla de Pagina!!!!----/n");
    core.conversion.IntToHexChar(dirFalla, string, 8);
    x86.ioScreen.printf("Dir de la Falla = %s /n", string);
    asm("movl 64(%ebp),%eax");
    asm("movl %%eax,%0" : "=m"(val));
    x86.ioScreen.printf("       ----- Process PID = %i -----/n",proceso->getpid());
    core.conversion.IntToHexChar(val, string, 4);
    x86.ioScreen.printf("         CS = %s : ", string);
    asm("movl 60(%ebp),%eax");
    asm("movl %%eax,%0" : "=m"(val));
    core.conversion.IntToHexChar(val, string, 8);
    x86.ioScreen.printf("EIP = %s /n", string);

    if (error == true) x86.ioScreen.printf("Esta fuera del espacio de usuario/n");
     core.adminProcess.killProcessRun();

}

void c_syscall() {
    u32 noSystemCall, parameter1, parameter2, parameter3, parameter4, parameter5, ret, *ebp;
    asm("movl %%ebp,%0" : "=m"(ebp));
    asm("movl %%eax,%0" : "=m"(noSystemCall));
    asm("movl %%ecx,%0" : "=m"(parameter1));
    asm("movl %%edx,%0" : "=m"(parameter2));
    asm("movl %%ebx,%0" : "=m"(parameter3));
    asm("movl %%esi,%0" : "=m"(parameter4));
    asm("movl %%edi,%0" : "=m"(parameter5));
    Cprocess *process = core.adminProcess.getRun();
    //Se salva el estado del proceso
    process->processX86.regX86->esp = ebp[17];
    process->processX86.regX86->edi = ebp[6];
    process->processX86.regX86->esi = ebp[7];
    process->processX86.regX86->ebp = ebp[8];
    process->processX86.regX86->ebx = ebp[10];
    process->processX86.regX86->edx = ebp[11];
    process->processX86.regX86->ecx = ebp[12];
    process->processX86.regX86->eax = ebp[13];
    process->processX86.regX86->cs = ebp[15];
    process->processX86.regX86->eflags = ebp[16];
    process->processX86.regX86->eip = ebp[14];
    ret = core.systemCall.systemCall(noSystemCall, parameter1, parameter2, parameter3,parameter4,parameter5);
    ebp[13] = ret;
}

void c_scheluder() {
    asm ("mov %0,%%eax" : "=m"(core.adminProcess.getRun()->processX86.regX86->cr3));
    asm ("mov %eax,%cr3");
    asm ("mov %0,%%esp" : "=m"(core.adminProcess.getRun()->processX86.regX86->esp));
    
    Cprocess *process = core.adminProcess.getRun();
    //Se prepara el cambio a modo de usuario
    asm ("mov %0,%%eax" : "=m"(process->processX86.regX86->esp));        
    asm ("push $0x33");                                          // SS
    asm ("push %eax");                                           //ESP
    asm ("push %0" : "=m"(process->processX86.regX86->eflags));  //EFLAGS
    asm ("push %0" : "=m"(process->processX86.regX86->cs));      //CS
    asm ("push %0" : "=m"(process->processX86.regX86->eip));     //IP
    //Se guardan los demas registros
    asm ("push %0" : "=m"(process->processX86.regX86->eax));
    asm ("push %0" : "=m"(process->processX86.regX86->ecx));
    asm ("push %0" : "=m"(process->processX86.regX86->edx));
    asm ("push %0" : "=m"(process->processX86.regX86->ebx));
    asm ("push %0" : "=m"(process->processX86.regX86->esp));
    asm ("push %0" : "=m"(process->processX86.regX86->ebp));
    asm ("push %0" : "=m"(process->processX86.regX86->esi));
    asm ("push %0" : "=m"(process->processX86.regX86->edi));
    asm ("push %0" : "=m"(process->processX86.regX86->ds));
    asm ("push %0" : "=m"(process->processX86.regX86->es));
    asm ("push %0" : "=m"(process->processX86.regX86->fs));
    asm ("push %0" : "=m"(process->processX86.regX86->gs));
    asm ("ljmp $0x08,$_asm_restore");
}