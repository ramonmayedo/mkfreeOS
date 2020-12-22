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


extern Score core;
extern Smaps maps;
extern Sx86 x86;

void c_passint() {
    x86.intHandler.eoi();
}
//Atencion al timer IRQ0

void c_int_timer() {
    u32 *ebp;
    asm volatile("mov %%ebp,%0" : "=m"(ebp));
    x86.intHandler.eoi();
    maps.ticks++;
    if (maps.ticks >= TICKS_PER_SECOND) {
        maps.clockMap.weakUp++;
        maps.ticks = 0;
        core.cacheDisk.flush();
    }
    if (maps.ticksKernelDelay > 0) maps.ticksKernelDelay--;
    core.adminProcess.tick(ebp);
}

void c_int_keyboard() {
    core.keyborad.IRQ();
    x86.intHandler.eoi();
}

void c_int_disk0() {
    x86.intHandler.eoi();
}

void c_mouse_ps2() {
    core.mousePS2.IRQ();
    x86.intHandler.eoi();
}

void c_gp() {
    asm volatile("movl 64(%ebp),%eax");
    asm volatile("movl %%eax,%0" : "=m"(maps.statusX86.cs));
    asm volatile("movl 60(%ebp),%eax");
    asm volatile("movl %%eax,%0" : "=m"(maps.statusX86.eip));
    Cprocess * process = core.adminProcess.getRun()->process;
    // Si noy hay procesos ready y el que falla es el que se esta ejecutando o es el kernel el que falla    
    if (process == 0 || core.adminProcess.getNext() == 0 || maps.statusX86.eip < VM_KERNEL_PAGE1_END)
        x86.architecture.kernelStopScreen("/n              !!!! GENERAL FAULT !!!!             /n");
    x86.architecture.kernelStopScreen("/n              !!!! GENERAL FAULT !!!!             /n");
    core.adminProcess.killProcessRun();
}

void c_page_fault() {
    u32 dirFalla;
    Cprocess *process;
    asm volatile("mov %cr2, %eax");
    asm volatile("mov %%eax, %0" : "=m"(dirFalla));
    asm volatile("mov %%eax, %0" : "=m"(maps.statusX86.cr2));
    asm volatile("movl 64(%ebp),%eax");
    asm volatile("movl %%eax,%0" : "=m"(maps.statusX86.cs));
    asm volatile("movl 60(%ebp),%eax");
    asm volatile("movl %%eax,%0" : "=m"(maps.statusX86.eip));
    process = (Cprocess*) core.adminProcess.getRun()->process;
    // Si noy hay procesos ready y el que falla es el que se esta ejecutando o es el kernel el que falla
    if (process == 0 || core.adminProcess.getNext() == 0)
        x86.architecture.kernelStopScreen("                      !!!PAGE FAULT!!!!                        /n");

    bool error = false;
    if (dirFalla <= VM_USER_PAGE || dirFalla >= VM_USER_PAGE_END)
        error = true;
    else {
        x86.virtualMemory.insertPageToPageDirectoryUser(process, (u8*) dirFalla, 0);
        return;
    }
    //Si la direccion de falla esta fuera del espacio de usuario se termina la ejecucion
    x86.architecture.kernelStopScreen("                      !!!PAGE FAULT!!!!                        /n");
    core.adminProcess.killProcessRun();
}

void c_syscall() {
    u32 noSystemCall, parameter1, parameter2, parameter3, parameter4, parameter5, ret, *ebp;
    asm volatile("movl %%ebp,%0" : "=m"(ebp));
    asm volatile("movl %%eax,%0" : "=m"(noSystemCall));
    asm volatile("movl %%ecx,%0" : "=m"(parameter1));
    asm volatile("movl %%edx,%0" : "=m"(parameter2));
    asm volatile("movl %%ebx,%0" : "=m"(parameter3));
    asm volatile("movl %%esi,%0" : "=m"(parameter4));
    asm volatile("movl %%edi,%0" : "=m"(parameter5));
    //Se salva el estado del proceso
    c_save_current_state(ebp, core.adminProcess.getRun());
    ret = core.systemCall.systemCall(noSystemCall, parameter1, parameter2, parameter3, parameter4, parameter5);
    ebp[13] = ret;
}

void c_scheluder(Cthread *thread, bool changeDirectoryPages) {
    if (changeDirectoryPages) {
        asm volatile("mov %0,%%eax" : "=m"(thread->process->processX86.cr3));
        asm volatile("mov %eax,%cr3");
    }
    asm volatile("mov %0,%%esp" : "=m"(thread->regX86->esp));
    //Se prepara el cambio a modo de usuario
    asm volatile("mov %0,%%eax" : "=m"(thread->regX86->esp));
    asm volatile("push $0x33"); // SS
    asm volatile("push %eax"); //ESP
    asm volatile("push %0" : "=m"(thread->regX86->eflags)); //EFLAGS
    asm volatile("push %0" : "=m"(thread->regX86->cs));     //CS
    asm volatile("push %0" : "=m"(thread->regX86->eip));    //IP
    //Se guardan los demas registros
    asm volatile("push %0" : "=m"(thread->regX86->eax));
    asm volatile("push %0" : "=m"(thread->regX86->ecx));
    asm volatile("push %0" : "=m"(thread->regX86->edx));
    asm volatile("push %0" : "=m"(thread->regX86->ebx));
    asm volatile("push %0" : "=m"(thread->regX86->esp));
    asm volatile("push %0" : "=m"(thread->regX86->ebp));
    asm volatile("push %0" : "=m"(thread->regX86->esi));
    asm volatile("push %0" : "=m"(thread->regX86->edi));
    asm volatile("push %0" : "=m"(thread->regX86->ds));
    asm volatile("push %0" : "=m"(thread->regX86->es));
    asm volatile("push %0" : "=m"(thread->regX86->fs));
    asm volatile("push %0" : "=m"(thread->regX86->gs));
    asm volatile("ljmp $0x08,$_asm_restore");
}
//Para salvar el estado del proceso en sus registros

void c_save_current_state(u32 *ebp, Cthread *thread) {
    thread->regX86->gs = ebp[2];
    thread->regX86->fs = ebp[3];
    thread->regX86->es = ebp[4];
    thread->regX86->ds = ebp[5];
    thread->regX86->edi = ebp[6];
    thread->regX86->esi = ebp[7];
    thread->regX86->ebp = ebp[8];
    thread->regX86->ebx = ebp[10];
    thread->regX86->edx = ebp[11];
    thread->regX86->ecx = ebp[12];
    thread->regX86->eax = ebp[13];
    thread->regX86->eip = ebp[14];
    thread->regX86->cs = ebp[15];
    thread->regX86->eflags = ebp[16];
    thread->regX86->esp = ebp[17];
}

void c_bootCpus() {
    x86.ioScreen.printf("CPU %i running /n", x86.intHandler.getLapicId());
    asm("cli");
    while (1);
}