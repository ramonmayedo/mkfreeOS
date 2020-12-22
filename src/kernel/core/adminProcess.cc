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

#include "adminProcess.h"
#include "../architecture/x86/x86.h"
#include "../architecture/x86/intfunction.h"

extern Score core;
extern Sx86 x86;
extern Smaps maps;

//DECLARE_LOCK(adminProcess);

CadminProcess::CadminProcess() {

}

void CadminProcess::initialize() {
    run = 0;
    max = 0;
    pid = 0;
    tid = 0;
    ready = new Clist;
    lock = new Clist;
    processList = new Clist;
}

int CadminProcess::addReady(char *afile, int argc, char **argv, int priority) {
    if (core.elfLoader.isValidElf((u8*) afile)) return 0;
    pid++;
    Cprocess *proceso = new Cprocess((u8*) afile, argc, argv, pid, priority);
    processList->add(proceso);
    return pid;
}

int CadminProcess::addThread(Cthread* thread) {
    tid++;
    thread->settid(tid);
    ready->add(thread);
    return tid;
}

void CadminProcess::schelude() {
    if (run != 0) {
        if (run->state == THREAD_RUN)
            ready->add(run);
        else
            lock->add(run);
    }
    if (ready->count() > 0) {
        bool changeDirectoryPages = true;
        Cthread* read = (Cthread*) ready->removeFirst();
        if (run->process == read->process)
            changeDirectoryPages = false;
        run = read;
        run->onActivate();
        c_scheluder(run, changeDirectoryPages);
    }
    run = 0;
    asm volatile("sti");
    while (1);
}

Cthread *CadminProcess::getRun() {
    return run;
}

Cthread *CadminProcess::getNext() {
    return (Cthread*) ready->getFirst();
}

int CadminProcess::deleteThread(int tid) {
    Cthread* thread;
    for (int i = 0; i < ready->count(); i++) {
        thread = (Cthread*) ready->getItem(i);
        if (thread->gettid() == tid) {
            ready->delet(i);
            delete(thread);
            return tid;
        }
    }
    return 0;
}

void CadminProcess::killProcessRun() {
        x86.architecture.changeToDirectoryKernel(); //Se cambia al dir del kernel
        deleteProcess(run->process);                //Se borran los hilos de la lista
        delete(run->process);                       //Se borra el proceso
        delete(run);                                //Se borra el RUN
        run = 0;
        schelude();                                 //Se hace la conmutacion de proceso
}

int CadminProcess::unlockThread(Cthread *thread, int priority) {
    if (isThreadExist(thread) == -1) return -1;    //Se verifica que el hilo no se haya eliminado
    if (thread->state == THREAD_BLOCK) {
        thread->state = THREAD_READY;
        lock->remove(thread);
        if (priority == ADP_PRIORITY_LOW)         //Prioridad bja se inserta al final de la lista
            ready->add(thread);
        else if (priority == ADP_PRIORITY_HIGHT)  //Prioridad alta se inserta en la cabeza de la pila
            ready->insert(0, thread);
    }
    return thread->gettid();
}

Cthread *CadminProcess::getThread(int tid) {
    Cthread* thread;
    for (int i = 0; i < lock->count(); i++) {
        thread = (Cthread*) lock->getItem(i);
        if (thread->gettid() == tid)
            return thread;
    }
    for (int i = 0; i < ready->count(); i++) {
        thread = (Cthread*) ready->getItem(i);
        if (thread->gettid() == tid)
            return thread;
    }
    if (run->gettid() == tid) return run;
    return 0;
}

Cprocess *CadminProcess::getProcess(int pid) {
    Cprocess* process;
    for (int i = 0; i < processList->count(); i++) {
        process = (Cprocess*) processList->getItem(i);
        if (process->getpid() == pid)
            return process;
    }
    return 0;
}

int CadminProcess::isThreadExist(Cthread* thread) {
    if (run == thread) return 0;
    int pid = ready->indexOf(thread); //Se busca en los ready
    if (pid == -1) //Si no existe se busca en los bloqueados
        pid = lock->indexOf(thread);
    return pid;
}

int CadminProcess::command(int acommand, int parameter1, int parameter2, int parameter3, int parameter4) {
    switch (acommand) {
        case cmpExecV:
        {
            char *fileName = (char*) parameter1;
            char **argv = (char**) parameter2;
            int argc = 0;
            if (argv != 0) {
                char **ptrArg = argv;
                while (*ptrArg++) argc++;
            }
            Cfile *newfile = new Cfile(fileName, FILE_R);
            int error = newfile->getError();
            if (error != 0) return 0;
            char *buffer = new char[newfile->getSize()];
            newfile->readAll(buffer);
            error = addReady(buffer, argc, argv, priorityNormal);
            delete(buffer);
            delete(newfile);
            return (u32) error;
        }
        case cmpExit:
        {
            killProcessRun();
            return 0;
        }
        case cmpSrbk:
        {
            u32 ret;
            ret = (u32) run->process->processX86.heap.end;
            run->process->processX86.heap.end += parameter1;
            return ret;
        }
        case cmpCreateThread:
        {
            return run->process->addThread(parameter1);
        }
        case cmpKillThread:
        {
            if (run->gettid() == parameter1 && run->getmainThred())
                killProcessRun();
            return deleteThread(parameter1);
        }
        case cmpSendCommand:
        {
            if (parameter2 == run->process->getpid()) return 0; //No se peude enviar a si mismo un mensaje
            Cprocess *process = getProcess(parameter2); //Se obtiene el proceso
            if (process == 0) return 0; //PID invalido 
            int idcommand = process->sendCommad(parameter1, run->process->getpid(), parameter3, parameter4);
            Cthread *thread = sendSignal(cmdCommandSleep, process); //Se envia la señal a ver que hilo espera
            if (thread) unlockThread(thread, ADP_PRIORITY_LOW); //Si hay un hilo se desbloquea
            return idcommand;
        }
        case cmpGetCommand:
        {
            int idcommand = run->process->getCommand(parameter1, parameter2, parameter3, parameter4); //Se busca un señal
            if (idcommand != -1) return idcommand; //Si hay se retorna
            run->waitSignal = cmdCommandSleep; //Si no se duerme el hilo       
            return run->sendState(cmdCommandSleep, 20);
        }
        case cmpGetPid:
        {
            return run->process->getpid();
        }
        case cmpReadBufferIPC:
        {
            Cprocess *process = getProcess(parameter1); //Se obtiene el proceso
            if (process == 0) return 0;
            int offset = parameter3 + parameter4; //offset + size
            if (offset >= VM_PAGE_SIZE) return 0;
            core.memory.memcpy((char*) parameter2, (process->getbufferIPC() + parameter3), parameter4);
            return parameter4;
        }
        case cmpWriteBufferIPC:
        {
            Cprocess *process = getProcess(parameter1); //Se obtiene el proceso
            if (process == 0) return 0;
            int offset = parameter3 + parameter4; //offset + size
            if (offset >= VM_PAGE_SIZE) return 0;
            core.memory.memcpy((process->getbufferIPC() + parameter3), (char*) parameter2, parameter4);
            return parameter4;
        }
        default: return 0;
    }
}

void CadminProcess::tick(u32 *ebp) {
    if (run != 0) {
        run->controlTime.beforeConsumedTime++;
        if (run->controlTime.beforeConsumedTime >= run->controlTime.plannedTime) {
            c_save_current_state(ebp, run);
            schelude();
        }
    } else
        if (ready->count()) {
        run = (Cthread*) ready->removeFirst();
        run->onActivate();
        c_scheluder(run, true);
    }
}

Cthread* CadminProcess::sendSignal(int signal, Cprocess *process) {
    Cthread *thread;
    for (int i = 0; i < lock->count(); i++) {
        thread = (Cthread*) lock->getItem(i);
        if (thread->waitSignal == signal && process == thread->process) {
            thread->waitSignal = 0;
            return thread;
        }
    }
    return 0;
}

void CadminProcess::deleteProcess(Cprocess* process) {
    Cthread *thread;
    processList->remove(process);
    for (int i = 0; i < lock->count(); i++) {
        thread = (Cthread*) lock->getItem(i);
        if (process == thread->process)
            delete(thread);
    }
    for (int i = 0; i < ready->count(); i++) {
        thread = (Cthread*) ready->getItem(i);
        if (process == thread->process)
            delete(thread);
    }
}