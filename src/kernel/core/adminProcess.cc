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
extern u32 asm_syscall_ret;

CadminProcess::CadminProcess() {
    run = 0;
    max = 0;
    id = 0;
}

int CadminProcess::addReady(char *afile, int argc, char **argv) {
    if (core.elfLoader.isValidElf((u8*) afile)) return 0;
    id++;
    Cprocess *proceso = new Cprocess((u8*) afile, argc, argv, id);
    proceso->setParent(run);
    //if (run != 0)
    //run->addProcessChild(proceso);
    ready.add(proceso);
    return id;
}

void CadminProcess::schelude() {
    if (ready.count() > 0) {
        Cprocess* read = (Cprocess*) ready.removeFirst();
        if (run != 0) {
            if (run->getcurrentState() == 0) ready.add(run);
            else lock.add(run);
            run->onDesactivate();
        }
        run = read;
        run->onActivate();
        c_scheluder();
    }
}

Cprocess *CadminProcess::getRun() {
    return run;
}

Cprocess *CadminProcess::getNext() {
    return (Cprocess*) ready.getFirst();
}

int CadminProcess::deleteProcess(int aidPID) {
    Cprocess* process;
    for (int i = 0; i < ready.count(); i++) {
        process = (Cprocess*) ready.getItem(i);
        if (process->getpid() == aidPID) {
            ready.delet(i);
            delete(process);
            return aidPID;
        }
    }
    return 0;
}

void CadminProcess::killProcessRun() {
    if (ready.count() > 0) {
        x86.architecture.changeToDirectoryKernel();
        delete(run);
        run = 0;
        schelude();
    }
    return;
}

int CadminProcess::unlockProcess(Cprocess *process, Cthread *thread) {
    process->adminThread->unlockThread(thread);
    if (process->getcurrentState() == PROC_BLOCK) {
        process->sendState(0);
        lock.remove(process);
        ready.add(process);
        return process->getpid();
    }
    return 0;
}


Cprocess *CadminProcess::getProcess(int aidPID) {
    Cprocess* process;
    for (int i = 0; i < lock.count(); i++) {
        process = (Cprocess*) lock.getItem(i);
        if (process->getpid() == aidPID)
            return process;
    }
    for (int i = 0; i < ready.count(); i++) {
        process = (Cprocess*) ready.getItem(i);
        if (process->getpid() == aidPID)
            return process;
    }
    if (run->getpid() == aidPID) return run;

    return 0;
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
            error = addReady(buffer, argc, argv);
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
            ret = (u32) run->processX86.heap.end;
            run->processX86.heap.end += parameter1;
            return ret;
        }
        case cmpCreateThread:
        {
            return run->addThread(parameter1);
        }
        case cmpKillThread:
        {
            return run->adminThread->killThread(parameter1);
        }
        case cmpSendCommand:
        {
            if (parameter2 == run->getpid()) return 0; //No se peude enviar a si mismo un mensaje
            Cprocess *process = getProcess(parameter2); //Se obtiene el proceso
            if (process == 0) return 0; //PID invalido 
            int idcommand = process->sendCommad(parameter1, run->getpid(), parameter3, parameter4);
            Cthread *thread = process->adminThread->sendSignal(cmdCommandSleep);
            if (thread) unlockProcess(process, thread);
            return idcommand;
        }
        case cmpGetCommand:
        {
            int idcommand = run->getCommand(parameter1, parameter2, parameter3, parameter4);
            if (idcommand) return idcommand;
            run->adminThread->getRun()->waitSignal = cmdCommandSleep;  
            return run->sendState(cmdCommandSleep);
        }
        case cmpGetPid:
        {
            return run->getpid();
        }
        case cmpWaitCommand:
        {
           /* int idcommand = run->getWaitCommand(parameter1,parameter2,parameter3,parameter4);
            if (idcommand) return idcommand;
            run->adminThread->getRun()->waitSignal = parameter1; 
            return run->sendState(cmdCommandSleep);    */
            return 0;
        }
        case cmpReadBufferIPC:
        {
            Cprocess *process = getProcess(parameter1); //Se obtiene el proceso
            if (process == 0) return 0;
            int offset = parameter3 + parameter4; //offset + size
            if (offset >= maps.memoryPagination.pageSize) return 0;
            core.memory.memcpy((char*) parameter2, (process->getbufferIPC() + parameter3), parameter4);
            return parameter4;
        }
        case cmpWriteBufferIPC:
        {
            Cprocess *process = getProcess(parameter1); //Se obtiene el proceso
            if (process == 0) return 0;
            int offset = parameter3 + parameter4; //offset + size
            if (offset >= maps.memoryPagination.pageSize) return 0;
            core.memory.memcpy((process->getbufferIPC() + parameter3), (char*) parameter2, parameter4);
            return parameter4;
        }
        default: return 0;
    }
}