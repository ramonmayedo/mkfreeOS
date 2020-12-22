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

#include "process.h"
#include "../architecture/x86/x86.h"

extern Smaps maps;
extern Sx86 x86;
extern Score core;

Cprocess::Cprocess(u8 *afile, int argc, char **argv, int apid, int apriority) {
    pid = apid;
    priority = apriority;
    countFiles = 3;
    char **param;
    SregX86 *regX86 = new SregX86;
    core.elfLoader.load(afile, &processX86); //Se carga el fichero elf
    //Copio los parametros del programa al nucleo del kernel
    if (argc) param = saveArgMainToKernel(argc, argv);
    //Se cambia al directorio del kernel     
    x86.architecture.changeToDirectoryKernel();
    //Se crea directorio de paginas del Proceso alamacena direcciones fisicas reales
    processX86.pageDirectory = x86.virtualMemory.createPageDirectoryUser(&processX86.cr3);
    //se cambia al directorio del nuevo proceso
    x86.architecture.setDirectory(processX86.cr3);
    //se crea pagina para directorio de pagina que almacena las direcciones virtuales de los marcos de página (Solo 4K !!!)
    processX86.pageDirectoryVirtual = (u32*) x86.virtualMemory.getPagesHeapToKernel();
    //cantidad de paginas del proceso
    int count = ((u32) (processX86.exec.end - processX86.exec.begin) / VM_PAGE_SIZE) + 1;
    sizeMemory = count;
    //se crea marco de pagina para el codigo
    u8* addressCode = processX86.exec.begin;
    for (int i = 0; i < count; i++) {
        x86.virtualMemory.insertPageToPageDirectoryUser(this, addressCode, 0);
        addressCode += VM_PAGE_SIZE;
    }
    //se copia el codigo
    for (int i = 0; i < processX86.exec.size; i++)
        processX86.exec.begin[i] = ((u8*) processX86.exec.offsetFile)[i];
    //Cantidad de Paginas del BSS
    int countBSS = ((u32) (processX86.bss.end - processX86.bss.begin) / VM_PAGE_SIZE) + 1;
    u8* addressBSS = processX86.bss.begin;
    for (int i = 0; i < countBSS; i++) {
        x86.virtualMemory.insertPageToPageDirectoryUser(this, addressBSS, 0);
        addressBSS += VM_PAGE_SIZE;
    }
    for (int i = 0; i < processX86.bss.size; i++)
        processX86.bss.begin[i] = ((u8*) processX86.bss.offsetFile)[i];
    //if (pid > 1) {x86.ioScreen.printf("bss %h %h", processX86.bss.begin, ((u32*)processX86.bss.begin)[5]);while(1);}
    //se copia los datos
    //Pila del Programa STACK    
    processX86.stack.begin = (u8*) (VM_USER_STACK + VM_PAGE_SIZE - 16);
    //Pila para los thread
    processX86.stackThread = (VM_USER_STACK - 64 * VM_PAGE_SIZE);
    //Se crea en el directorio de paginas la pagina para la Pila
    x86.virtualMemory.insertPageToPageDirectoryUser(this, processX86.stack.begin, 0);
    //Se salvan los parametros del main(int argc,char **argv) en la pila del nuevo proceso
    processX86.stack.begin = saveArgMain(argc, param, (char*) processX86.stack.begin);
    //Se asigna el area para el heap despues de bss
    processX86.heap.begin = (u8*) (((u32) processX86.bss.end & 0xFFFFF000) + VM_PAGE_SIZE);
    processX86.heap.end = processX86.heap.begin;
    //Buffer minimo para el intercambio de informacion entre procesos
    bufferIPC = (char*) x86.virtualMemory.getPagesHeapToKernel();
    regX86->eax = 0;
    regX86->ebx = 0;
    regX86->ecx = 0;
    regX86->edx = 0;
    regX86->esi = 0;
    regX86->edi = 0;
    regX86->esp = (u32) processX86.stack.begin;
    regX86->eip = (u32) processX86.exec.begin;
    regX86->ebp = 0;
    regX86->cs = 0x23;
    regX86->ds = 0x2B;
    regX86->es = 0x2B;
    regX86->fs = 0x2B;
    regX86->ss = 0x33;
    regX86->gs = 0x2B;
    regX86->eflags = 0x202;
    //Se crea como nuevo hilo de ejecucion que es el principal
    core.adminProcess.addThread(new Cthread(regX86, this, true));

    if (core.adminProcess.getRun() != 0) //Si es un proceso el que crea otro proceso
        x86.architecture.changeToDirectoryCurrentProcess(); //Se vuelve al proceso que lo creo
    //se eliminan los parametros que se copiaron al kernel desde el proceso
    if (argc) {
        for (int i = 0; i < argc; i++)
            delete(param[i]);
        delete(param);
    }
    //comandos para la comunicacion entre proceso
    commands = new ScommandProcess[MAX_COMMANDS];
    for (int i = 0; i < MAX_COMMANDS; i++) commands[i].command = -1;
}

int Cprocess::getpid() {
    return pid;
}

Cprocess::~Cprocess() {
    x86.virtualMemory.deleteDirectoryPagesUser(processX86.pageDirectoryVirtual); //Se borran las entradas del directorio de paginas
    x86.virtualMemory.releasePagesHeapToKernel((u8*) processX86.pageDirectoryVirtual); //Se borra pagina del directorio virtual
    x86.virtualMemory.releasePagesHeapToKernel((u8*) processX86.pageDirectory); //Se borra pagina del directorio fisico
    x86.virtualMemory.releasePagesHeapToKernel((u8*) bufferIPC);
    deleteAllFiles(); //Se borran todos los archivos abiertos
    //deleteAllProcessChild();
    delete(commands);
}

void Cprocess::deleteAllFiles() {
    Cfile* filesProcess;
    for (int i = 0; i < files.count(); i++) {
        filesProcess = (Cfile*) files.getItem(i);
        delete(filesProcess);
    }
    files.clear();
}

u8* Cprocess::saveArgMain(int argc, char **argv, char *stackPtr) {
    char **parametersStack = new char*[argc + 1];
    int lenght, i;

    for (i = 0; i < argc; i++) {
        lenght = core.string.strLenght(argv[i]); //tamaño del parametro
        stackPtr -= lenght + 1; //apunto al tope de la pila para luego copiar
        core.string.strCopy(stackPtr, argv[i], lenght);
        parametersStack[i] = (char*) stackPtr;
    }

    stackPtr -= sizeof (char*);

    *((char**) stackPtr) = 0x0; //ultimo parametro = 0x0

    for (i = argc - 1; i >= 0; i--) { //se guarda la tabla de **argv
        stackPtr -= sizeof (char*);
        *((char**) stackPtr) = parametersStack[i];
    }
    stackPtr -= sizeof (char*); //se almacena el parametro char **argv
    *((char**) stackPtr) = (char*) (stackPtr + 4);

    stackPtr -= sizeof (char*); //Se almacena el parametro int argc
    *((int*) stackPtr) = argc;

    stackPtr -= sizeof (char*);

    delete(parametersStack);

    return (u8*) stackPtr;
}

char** Cprocess::saveArgMainToKernel(int argc, char **argv) {
    char **param = new char*[argc + 1];
    int i, lenght;
    for (i = 0; i < argc; i++) {
        lenght = core.string.strLenght(argv[i]);
        param[i] = new char[lenght + 1];
        core.string.strCopy(param[i], argv[i], lenght);
    }
    param[i] = 0;
    return param;
}

int Cprocess::addFile(void* afile) {
    countFiles++;
    ((Cfile*) afile)->setIdFile(countFiles);
    files.add(afile);
    return countFiles;
}

void *Cprocess::getFile(int aidFile) {
    Cfile *filesProcess;
    for (int i = 0; i < files.count(); i++) {
        filesProcess = (Cfile*) files.getItem(i);
        if (filesProcess->getIdFile() == aidFile) return (void*) filesProcess;
    }
    return 0;
}

int Cprocess::deleteFile(int aidFile) {
    Cfile *filesProcess;
    filesProcess = (Cfile*) getFile(aidFile);
    if (filesProcess) {
        files.remove(filesProcess);
        delete(filesProcess);
        return aidFile;
    }
    return 0;
}

void Cprocess::setParent(Cprocess *aparent) {
    processParent = aparent;
}

Cprocess *Cprocess::getParent() {
    return processParent;
}

int Cprocess::addProcessChild(Cprocess *achild) {
    return processChild.add(achild);
}

int Cprocess::deleteProcessChild(Cprocess *achild) {
    return processChild.remove(achild);
}

int Cprocess::deleteAllProcessChild() {
    Cprocess *child;
    while (processChild.count()) {
        child = (Cprocess*) processChild.getFirst();
        core.adminProcess.deleteThread(child->getpid());
    }
    if (processParent != 0) processParent->deleteProcessChild(this);
    processChild.clear();
}

char* Cprocess::getbufferIPC() {
    return bufferIPC;
}

int Cprocess::addThread(u32 eip) {
    SregX86 *regX86 = new SregX86;
    regX86->eip = eip;
    processX86.stackThread -= VM_PAGE_SIZE * 32;
    regX86->esp = (processX86.stackThread - 16);
    x86.virtualMemory.insertPageToPageDirectoryUser(this, (u8*) regX86->esp, 0);
    regX86->eax = 0;
    regX86->ebx = 0;
    regX86->ecx = 0;
    regX86->edx = 0;
    regX86->esi = 0;
    regX86->edi = 0;
    regX86->ebp = 0;
    regX86->cs = 0x23;
    regX86->ds = 0x2B;
    regX86->es = 0x2B;
    regX86->fs = 0x2B;
    regX86->ss = 0x33;
    regX86->gs = 0x2B;
    regX86->eflags = 0x202;
    return core.adminProcess.addThread(new Cthread(regX86, this, false));
}

int Cprocess::getCommand(int command, int parameter1, int parameter2, int parameter3) {
    int temp;
    for (int i = 0; i < MAX_COMMANDS; i++) {
        if (commands[i].command != -1) {
            *(int*) command = commands[i].command;
            *(int*) parameter1 = commands[i].pidTrasmisor;
            *(int*) parameter2 = commands[i].parameter1;
            *(int*) parameter3 = commands[i].parameter2;
            commands[i].command = -1;
            return i;
        }
    }
    return -1;
}

int Cprocess::sendCommad(int command, int parameter1, int parameter2, int parameter3) {
    for (int i = 0; i < MAX_COMMANDS; i++) {
        if (commands[i].command == -1) {
            commands[i].command = command;
            commands[i].pidTrasmisor = parameter1;
            commands[i].parameter1 = parameter2;
            commands[i].parameter2 = parameter3;
            return i;
        }
    }
    return 0;
}