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
    initBitapMemoryPages();
    initDirectoryPages();
    initMemoryPagesVirtual();
}

void CvirtualMemory::initDirectoryPages() {
    pagDirectory = (u32*) maps.memoryPagination.directoryPageKernel;
    u8 *pg0 = (u8*) 0;
    u8 *pg1 = (u8*) maps.memoryPagination.kernelPage1; //direccion del la paginas del kernel
    //Se inicializa el directorio de paginas del kernel     
    pagDirectory[0] = ((u32) (pg0) | PG_PRESENT | PG_WRITE | PG_4MB); //pagina 0 coincide con memoria fisica 0x00000000 - 0x00400000 0-4MB
    pagDirectory[1] = ((u32) (pg1) | PG_PRESENT | PG_WRITE | PG_4MB); //pagina 1 coincide con memoria fisica 0x00400000 - 0x00800000 4MB-8MB

    for (int i = 2; i < 1024; i++)
        pagDirectory[i] = ((u32) pg1 + (PAGE_SIZE * i)) | (PG_PRESENT | PG_WRITE);
          
    asm("mov %0, %%eax \n \
	 mov %%eax, %%cr3 \n \
	 mov %%cr4, %%eax \n \
	 or $0x00000010, %%eax \n \
	 mov %%eax, %%cr4 \n \
	 mov %%cr0, %%eax \n \
	 or $0x80000000, %%eax \n \
	 mov %%eax, %%cr0" : "=m"((u32) maps.memoryPagination.directoryPageKernel));
    
    //Se reserva memoria para uso del kernel
    maps.memoryPagination.kernelHeapPtr = maps.memoryPagination.kernelHeap;
    x86.malloc.bsrk(1);
}

void CvirtualMemory::initBitapMemoryPages() {
    bitMapPages = maps.memoryPagination.bitMapPages;
    //Estas paginas son mapeadas y existen fisicamente y se definen como no ocupadas   
    u32 allPagesPhisic = maps.memoryPagination.ramPhisic / maps.memoryPagination.pageSize;

    for (int i = 0; i < allPagesPhisic / 8; i++)
        bitMapPages[i] = 0x0;

    //Estas paginas no existen fisicamente y se definen como ocupadas
    for (int i = allPagesPhisic / 8; i < maps.memoryPagination.ramMaxPage / 8; i++)
        bitMapPages[i] = 0xff;

    //Para obtener la cantidad real de paginas que ya tengo ocupada y asignarla al mapa de bit de paginas
    u32 realPageOcuped = maps.memoryPagination.kernelEnd / maps.memoryPagination.pageSize;
    int bitMask = 0x1, j = 0;
    for (int i = 0; i < realPageOcuped; i++) {
        bitMapPages[j] |= bitMask;
        bitMask = bitMask << 1; //se corre la mascara << 1 es lo  mismo *2
        if (bitMask > 0x80) {
            bitMask = 0x1;
            j++;
        } //se ocuparon los 8 bit se coge una nueva entrada y se reinicia la mascara
    }
}

u8 * CvirtualMemory::getPageFrameToBitmap() {
    //Hay que pensar en optimizar
    int newPageAddress = -1;
    for (u32 i = 0; i < maps.memoryPagination.ramMaxPage / 8; i++) {
        if (bitMapPages[i] != 0xFF) {
            int bitMask = 0x1;
            for (u32 j = 0; j < 8; j++) {
                if ((bitMapPages[i] & bitMask) == 0) {
                    newPageAddress = ((8 * i) + j) * maps.memoryPagination.pageSize;
                    bitMapPages[i] |= bitMask;
                    return (u8*) newPageAddress;
                }
                bitMask = bitMask << 1;
            }
        }
    }
    return (u8*) newPageAddress;
}

int CvirtualMemory::addDirectoryPagesKernel(u8 *avirtualAddress, u8 *aphisicalAddress, u32 aflags) {
    return directoryPages(pagDirectory, avirtualAddress, aphisicalAddress, aflags, WRITE_PAGE);
}

int CvirtualMemory::directoryPages(u32 *apagDirectory, u8 *avirtualAddress, u8 *aphisicalAddress, u32 aflags, int acommand) {
    u32 indexTableDir, indexTablePage, attributes;
    u32 *phisicalAddressTablePagePtr;

    indexTableDir = ((u32) (avirtualAddress)) >> 22; //se optiene indice al directorio de paginas del kernel
    if ((apagDirectory[indexTableDir] & PG_PRESENT) == 0) {//se verifica que este presente el marco de pagina
        x86.ioScreen.printf("Error no esta Presente el Marco de Paginas/n");
        return 0;
    }
    //Direccion fisica donde comienza el marco de pagina de la entrada anterior
    phisicalAddressTablePagePtr = (u32*) ((apagDirectory[indexTableDir] & 0xFFFFF000));
    //Index de entrada en el marco de pagina
    indexTablePage = (((u32) avirtualAddress & 0x003FF000) >> 12);
    //Se asigna el valor real de la direccion fisica a la entrada del marco de pagina
    if (acommand == WRITE_PAGE) {
        attributes = (PG_PRESENT | PG_WRITE | aflags);
        phisicalAddressTablePagePtr[indexTablePage] = (((u32) aphisicalAddress & 0xFFFFF000) | attributes);
        for (char* m = (char*) ((u32) avirtualAddress & 0xFFFFF000), n = 0; n < maps.memoryPagination.pageSize; n++, m++)*m = 0x0;
     } else if (acommand == RELEASE_PAGE) {
        u32 phisicalAddress = phisicalAddressTablePagePtr[indexTablePage]&0xFFFFF000;
        phisicalAddressTablePagePtr[indexTablePage] = 0;
        int indexBitmap = (u32) phisicalAddress / maps.memoryPagination.pageSize;
        int indexBit = indexBitmap % 8;
        indexBitmap /= 8;
        bitMapPages[indexBitmap] ^= 1 << (indexBit);
    }
}

//Para asignar paginas del heap del kernel a los procesos solamente por eso se define una zona de memoria virtual
void CvirtualMemory::initMemoryPagesVirtual() {
    pagesKernel = new CvirtualMemoryZone(maps.memoryPagination.bitMapPagesKernel, maps.memoryPagination.kernelPageHeap, maps.memoryPagination.kernelPageHeapEnd, maps.memoryPagination.pageSize, pagDirectory);
}

u8* CvirtualMemory::getPagesVirtualKernel(u32 *phisicalPage) {
    return pagesKernel->getPageVirtualToBitmap(phisicalPage);
}

u8* CvirtualMemory::getPagesVirtualKernel() {
    u32 phisicalPage;
    return pagesKernel->getPageVirtualToBitmap(&phisicalPage);
}

int CvirtualMemory::releasePageVirtualKernel(u8 *avirtualAddress) {
    return pagesKernel->releasePageVirtual(avirtualAddress);
}

//Para trabajo con el directorio de paginas de los usuarios solamente
u32 *CvirtualMemory::createPageDirectory(u32 *phisicalAddress) { //Se crea directorio de pagina del usuario
    u32 *newPagDirectory = (u32*) getPagesVirtualKernel(phisicalAddress);
    for (int i = 0; i < 1024; i++) {
        if (i < 256 || i>895) newPagDirectory[i] = pagDirectory[i]; //para espacio de codigo igual al kernel solo permiso anillo 0 si entra
        else newPagDirectory[i] = 0;                              //en modo usuario a alguna direccion se produce un fallo de pagina
    }
    return newPagDirectory;
}

//Se crea una pagina dentro del directorio, de no existir un marco de pagina para esa pagina se crea tambien
int CvirtualMemory::createPageToDirectory(u32 *apagDirectory, u32 *apagDirectoryVirtual, u8 *avirtualAddress, u8 *aphisicalAdress) {
    u32 pageAddress, phisicalAddressD, indexTableDir = ((u32) (avirtualAddress)) >> 22;
    if ((apagDirectory[indexTableDir] & PG_PRESENT) == 0) {//se verifica que este presente el marco de pagina sino se crea
        pageAddress = (u32) getPagesVirtualKernel(&phisicalAddressD); //Se obtiene una pagina
        apagDirectory[indexTableDir] = ((phisicalAddressD & 0xFFFFF000) | PG_PRESENT | PG_WRITE | PG_USER); //se crea el marco pagina    
        apagDirectoryVirtual[indexTableDir] = ((pageAddress & 0xFFFFF000) | PG_PRESENT | PG_WRITE | PG_USER);
    } else pageAddress = apagDirectoryVirtual[indexTableDir]; //pagina virtual donde esta el marco de pagina
    //se agraga nueva pagina
    u8 * phisicalAddress;
    if (aphisicalAdress == 0)
        phisicalAddress = x86.virtualMemory.getPageFrameToBitmap(); //nueva pagina fisica
    else
        phisicalAddress = aphisicalAdress; 
    //directoryPages(pagDirectory, avirtualAddress, phisicalAddress, 0, WRITE_PAGE);//se registra en el dir del kernel
    u32 *phisicalAddressTablePagePtr = (u32*) (pageAddress & 0xFFFFF000); //dir de la tabla de paginas
    u32 indexTablePage = (((u32) avirtualAddress & 0x003FF000) >> 12); //index para cambiar
    phisicalAddressTablePagePtr[indexTablePage] = (((u32) phisicalAddress & 0xFFFFF000) | PG_PRESENT | PG_WRITE | PG_USER);
    //se inicializa la pagina en 0x0
    if (aphisicalAdress == 0)
        core.memory.memset((char*) ((u32) avirtualAddress & 0xFFFFF000), 0x0, maps.memoryPagination.pageSize);
    return 0;
}
//Se borran las paginas asignadas a los marcos de pagina 
void CvirtualMemory::deleteDirectoryPages(u32 *apagDirectory) {
    for (int i = 256; i < 1024; i++) {
        if ((apagDirectory[i] & PG_PRESENT) != 0) {
            apagDirectory[i] = 0x0;
            releasePageVirtualKernel((u8*) apagDirectory[i]);
        }
    }
}