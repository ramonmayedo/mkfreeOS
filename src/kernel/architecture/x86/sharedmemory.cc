#include "sharedmemory.h"
#include "x86.h"

extern Sx86 x86;
extern Score core;

CsharedMemory::CsharedMemory() {

}

void CsharedMemory::initialized() {
  sharedMemory = new Clist();
  idCount=1;  
}


int CsharedMemory::addSharedMemory(int idProcess, int flags, int password, u8* virtualMemory, u8* phisicalAdress) {
    SsharedMemory *newCharedMemory = new SsharedMemory();
    newCharedMemory->idProcessFather = idProcess;
    newCharedMemory->flags = flags;
    newCharedMemory->password = password;
    newCharedMemory->phisicalAdress = phisicalAdress;
    newCharedMemory->virtualMemory = virtualMemory;
    newCharedMemory->id = idCount;
    sharedMemory->add(newCharedMemory);
    idCount++;
}

int CsharedMemory::deleteSharedMemory(int idProcess, int idSharedMemory) {
    SsharedMemory *sharedMemoryF;
    for (int i = 0; i < sharedMemory->count(); i++) {
        sharedMemoryF = (SsharedMemory*) sharedMemory->getItem(i);
        if (sharedMemoryF->id == idSharedMemory && sharedMemoryF->idProcessFather == idProcess) {
            delete(sharedMemoryF);
            sharedMemory->delet(i);
            return idSharedMemory;
        }
    }
    return 0;
}

int CsharedMemory::setSharedMemory(int idProcess, int idSharedMemory) {
    SsharedMemory *sharedMemoryF;
    Cprocess *process = core.adminProcess.getRun();
    u32 *directory = process->processX86.pageDirectory;
    u32 *directoryVirtual = process->processX86.pageDirectoryVirtual;
    for (int i = 0; i < sharedMemory->count(); i++) {
        sharedMemoryF = (SsharedMemory*) sharedMemory->getItem(i);
        if (sharedMemoryF->id == idSharedMemory && sharedMemoryF->idProcessFather && idProcess) {
            x86.virtualMemory.createPageToDirectory(directory, directoryVirtual, sharedMemoryF->virtualMemory, sharedMemoryF->phisicalAdress);
            return (int)sharedMemoryF->virtualMemory;
        }
    }
    return 0;
}

int CsharedMemory::deleteSharedMemory(int idProcess) {
    SsharedMemory *sharedMemoryF;
    for (int i = 0; i < sharedMemory->count(); i++) {
        sharedMemoryF = (SsharedMemory*) sharedMemory->getItem(i);
        delete(sharedMemoryF);
    }
    //Falta borrar
    return 0;
}

int CsharedMemory::command(int acommand, int parameter1, int parameter2,int parameter3) {
    Cprocess *process = core.adminProcess.getRun();
    switch (acommand) {
        case cmsGetZone:
        {
            return process->sharedMemory;
        }
        case cmsAddZone:
        {
            int i;
            int tPages = (parameter3 / PAGE_SIZE) + 1;
            int ptrSharedMemory = setSharedMemory(parameter1, parameter2);
            for (i = 1; i < tPages; i++) {
                setSharedMemory(parameter1, parameter2 + i);
            }
            return ptrSharedMemory;
        }
        case cmsSharedZone:
        {
            u32 *directory = process->processX86.pageDirectory;
            u32 *directoryVirtual = process->processX86.pageDirectoryVirtual;
            int tPages = (parameter1 / PAGE_SIZE) + 1;
            int idInit = idCount;

            for (int i = 0; i < tPages; i++) {
                u8 *phisicalAddress = x86.virtualMemory.getPageFrameToBitmap();
                x86.virtualMemory.createPageToDirectory(directory, directoryVirtual, (u8*) process->sharedMemory, phisicalAddress);
                addSharedMemory(process->getpid(), 0, 0, (u8*) process->sharedMemory, phisicalAddress);
                process->sharedMemory = process->sharedMemory + PAGE_SIZE;
            }
            return idInit;
        }
        default: break;
    }
}
