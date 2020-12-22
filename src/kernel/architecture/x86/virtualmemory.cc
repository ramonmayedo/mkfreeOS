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

#include "virtualmemory.h"
#include "x86.h"

extern Smaps maps;
extern Sx86 x86;
extern Score core;

CvirtualMemory::CvirtualMemory() {

}

void CvirtualMemory::initialize() {
    maps.memoryPagination.ramMaxSize = VM_RAM_MAX_SIZE;
    maps.memoryPagination.ramPhisic = 0x04000000; //64 MByte
    phisicalBitmap.setBitmap(maps.memoryPagination.bitMapPages, maps.memoryPagination.ramPhisic / VM_PAGE_SIZE);
    phisicalBitmap.initializedBitmap(VM_KERNEL_END / VM_PAGE_SIZE);
    initDirectoryPages();
    kernelPageHeap = new CmemoryZone(VM_KERNEL_PAGE_HEAP, VM_KERNEL_PAGE_HEP_END, maps.memoryPagination.bitMapPagesKernel);
    sharedMemoryPage = new CmemoryZone(VM_USER_PAGE_SHARED, VM_USER_PAGE_SHARED_END, maps.memoryPagination.bitMapSharedMemory);
    idCountRegion = 0;
}

void CvirtualMemory::initDirectoryPages() {
    dirPagesKernel = (u32*) VM_KERNEL_PAGE_DIR;
    u32 pg0 =  0;
    u32 pg1 =  VM_KERNEL_PAGE1; //direccion del la paginas del kernel
    //Se inicializa el directorio de paginas del kernel     
    dirPagesKernel[0] = (pg0 | WM_PG_PRESENT | WM_PG_WRITE | WM_PG_4MB); //pagina 0 coincide con memoria fisica 0x00000000 - 0x00400000 0-4MB
    dirPagesKernel[1] = (pg1 | WM_PG_PRESENT | WM_PG_WRITE | WM_PG_4MB); //pagina 1 coincide con memoria fisica 0x00400000 - 0x00800000 4MB-8MB

    for (int i = 2; i < 1024; i++)
        dirPagesKernel[i] = (pg1 + (VM_PAGE_SIZE * i)) | (WM_PG_PRESENT | WM_PG_WRITE);
          
    asm("mov %0, %%eax \n \
	 mov %%eax, %%cr3 \n \
	 mov %%cr4, %%eax \n \
	 or $0x00000010, %%eax \n \
	 mov %%eax, %%cr4 \n \
	 mov %%cr0, %%eax \n \
	 or $0x80000000, %%eax \n \
	 mov %%eax, %%cr0" : "=m"((u32)dirPagesKernel));

    maps.memoryPagination.kernelHeapPtr = VM_KERNEL_HEAP; //Se reserva memoria para uso del kernel
    x86.malloc.bsrk(1); 
}

u8 * CvirtualMemory::getPhisicalPage() {
    return phisicalBitmap.getPageToBitmap();
}

int CvirtualMemory::addDirectoryPagesKernel(u8 *avirtualAddress, u8 *aphisicalAddress, u32 aflags) {
    return directoryPagesKernel(avirtualAddress, aphisicalAddress, aflags, WM_WRITE_PAGE);
}

int CvirtualMemory::directoryPagesKernel(u8 *avirtualAddress, u8 *aphisicalAddress, u32 aflags, int acommand) {
    u32 indexTableDir, indexTablePage, attributes;
    u32 *phisicalAddressTablePagePtr;

    indexTableDir = ((u32) (avirtualAddress)) >> 22; //se optiene indice al directorio de paginas
    if ((dirPagesKernel[indexTableDir] & WM_PG_PRESENT) == 0) //se verifica que este presente el marco de pagina
        x86.architecture.kernelStopScreen("KERNEL Virtual Memory Error ,Kernel Page Frame Not Present/n");
    //Direccion fisica donde comienza el marco de pagina de la entrada anterior
    phisicalAddressTablePagePtr = (u32*) ((dirPagesKernel[indexTableDir] & 0xFFFFF000));
    //Index de entrada en el marco de pagina
    indexTablePage = (((u32) avirtualAddress & 0x003FF000) >> 12);
    //Se asigna el valor real de la direccion fisica a la entrada del marco de pagina
    if (acommand == WM_WRITE_PAGE) {
        attributes = (WM_PG_PRESENT | WM_PG_WRITE | aflags);
        phisicalAddressTablePagePtr[indexTablePage] = (((u32) aphisicalAddress & 0xFFFFF000) | attributes);
    } else if (acommand == WM_RELEASE_PAGE) {
        u32 phisicalAddress = phisicalAddressTablePagePtr[indexTablePage]&0xFFFFF000;
        phisicalAddressTablePagePtr[indexTablePage] = 0;
        phisicalBitmap.relasePageToBitmap((u8*) phisicalAddress);
    }
}

//Para asignar paginas del heap del kernel a los procesos solamente por eso se define una zona de memoria virtual

u8* CvirtualMemory::getPagesHeapToKernel(u32 *phisicalPage) {
    u8 *phisicalAdress = phisicalBitmap.getPageToBitmap();
    *phisicalPage = (u32) phisicalAdress;
    u8 *virtualAdress = kernelPageHeap->getPageToMemoryZone();
    directoryPagesKernel(virtualAdress, phisicalAdress, NOT_FLAGS, WM_WRITE_PAGE);
    core.memory.memset((char*) virtualAdress, 0x0, VM_PAGE_SIZE);
    return virtualAdress;
}

u8* CvirtualMemory::getPagesHeapToKernel() {
    u8 *phisicalAdress = phisicalBitmap.getPageToBitmap();
    u8 *virtualAdress = kernelPageHeap->getPageToMemoryZone();
    directoryPagesKernel(virtualAdress, phisicalAdress, NOT_FLAGS, WM_WRITE_PAGE);
    core.memory.memset((char*) virtualAdress, 0x0, VM_PAGE_SIZE);
    return virtualAdress;
}

int CvirtualMemory::releasePagesHeapToKernel(u8 *avirtualAddress) {
    directoryPagesKernel(avirtualAddress, 0, 0, WM_RELEASE_PAGE); //Se libera la pagina del kernel
    return kernelPageHeap->relasePageToMemoryZone(avirtualAddress);
}

//--------------------------------------------------------------------------------------------------------
//Para trabajo con el directorio de paginas de los usuarios solamente
u32 *CvirtualMemory::createPageDirectoryUser(u32 *phisicalAddress) { //Se crea directorio de pagina del usuario
    u32 *newPagDirectory = (u32*) getPagesHeapToKernel(phisicalAddress);
    for (int i = 0; i < 1024; i++) {
        if (i < 256 || i > 768) newPagDirectory[i] = dirPagesKernel[i]; //para espacio de codigo igual al kernel solo permiso anillo 0 si entra
        else newPagDirectory[i] = 0;                                    //en modo usuario a alguna direccion se produce un fallo de pagina
    }
    return newPagDirectory;
}

//Se crea una pagina dentro del directorio, de no existir un marco de pagina para esa pagina se crea tambien
int CvirtualMemory::insertPageToPageDirectoryUser(Cprocess *process, u8 *avirtualAddress, u8 *aphisicalAdress) {
    u32 *apagDirectory = process->processX86.pageDirectory;
    u32 *apagDirectoryVirtual = process->processX86.pageDirectoryVirtual;
    u32 pageAddress, phisicalAddressD, indexTableDir = ((u32) (avirtualAddress)) >> 22;
    if ((apagDirectory[indexTableDir] & WM_PG_PRESENT) == 0) {//se verifica que este presente el marco de pagina sino se crea
        pageAddress = (u32) getPagesHeapToKernel(&phisicalAddressD); //Se obtiene una pagina
        apagDirectory[indexTableDir] = ((phisicalAddressD & 0xFFFFF000) | WM_PG_PRESENT | WM_PG_WRITE | WM_PG_USER); //se crea el marco pagina    
        apagDirectoryVirtual[indexTableDir] = ((pageAddress & 0xFFFFF000) | WM_PG_PRESENT | WM_PG_WRITE | WM_PG_USER);
    } else pageAddress = apagDirectoryVirtual[indexTableDir]; //pagina virtual donde esta el marco de pagina
    //se agraga nueva pagina
    u8 * phisicalAddress;
    if (aphisicalAdress == 0)
        phisicalAddress = phisicalBitmap.getPageToBitmap(); //nueva pagina fisica
    else
        phisicalAddress = aphisicalAdress; 
    u32 *phisicalAddressTablePagePtr = (u32*) (pageAddress & 0xFFFFF000); //dir de la tabla de paginas
    u32 indexTablePage = (((u32) avirtualAddress & 0x003FF000) >> 12); //index para cambiar
    phisicalAddressTablePagePtr[indexTablePage] = (((u32) phisicalAddress & 0xFFFFF000) | WM_PG_PRESENT | WM_PG_WRITE | WM_PG_USER);
    //se inicializa la pagina en 0x0
    if (aphisicalAdress == 0)
        core.memory.memset((char*) ((u32) avirtualAddress & 0xFFFFF000), 0x0, VM_PAGE_SIZE);
    return 0;
}

//Se borran las paginas asignadas a los marcos de pagina 
void CvirtualMemory::deleteDirectoryPagesUser(u32 *apagDirectory) {
    u32 *addressPageTable;
    for (int i = 256; i < 728; i++) { //Desde espacio de usuario 256 * 4Mb = 0x40000000 - 0xA0000000 (640*4MB)   
        if ((apagDirectory[i] & WM_PG_PRESENT) != 0) {
            addressPageTable = (u32*) (apagDirectory[i] & 0xFFFFF000); //Dir del marco de pagina
            if (i < 640)
                for (int j = 0; j < 1024; j++) { //Se revisan si estan asignadas paginas fisicas
                    if (addressPageTable[j] & WM_PG_PRESENT)//Si esta asignada la pagina
                        phisicalBitmap.relasePageToBitmap((u8*) addressPageTable[j]); //Se libera la pagina fisica
                }
            releasePagesHeapToKernel((u8*) apagDirectory[i]); //Se libera la pagina virtual del kernel
        }
    }
}

void CvirtualMemory::addRegion(CmemoryRegion* region) {
    regions.add(region);
     idCountRegion++;
}

int CvirtualMemory::command(int acommand, int parameter1, int parameter2, int parameter3) {
     Cprocess *process = core.adminProcess.getRun()->process;
    switch (acommand) {
        case cmsGetZone:
        {
            int tPages = (parameter1 / VM_PAGE_SIZE) + 1;
            u32 virtualAdress = (u32) sharedMemoryPage->getPageToMemoryZone(tPages);
            CmemoryRegion *newRegion = new CmemoryRegion(virtualAdress, virtualAdress, tPages, false);
            newRegion->setMappedKernelPhisV(NOT_FLAGS);
            newRegion->setMappedUser(process, NOT_FLAGS);    
            newRegion->key = idCountRegion;
            newRegion->isShared = true;
            regions.add(newRegion);
            *(int*) parameter2 = virtualAdress;
            idCountRegion++;
            return newRegion->key;
        }
        case cmsSharedZone:
        {
            CmemoryRegion *region;
            if (parameter1 >= regions.count())
                return 0;
            region = (CmemoryRegion*) regions.getItem(parameter1);
            if (region->key == parameter1) {
                region->setMappedUser(process, NOT_FLAGS);
                *(int*) parameter2 = region->vAddresStart;
                *(int*) parameter3 = region->size;
                return region->vAddresStart;
            }
            return 0;
        }
        default: break;
    }
}