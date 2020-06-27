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

#include "x86.h"

#include "../../core/kernel.h"

//Mapa de memoria del kernel se almacena toda la informacion respecto a la arquitectura
Smaps maps;
//Clases globales para uso del kernel (Para arquitectura x86, 32bit)
Sx86 x86;
//Clases globales para uso del kernel (Para todas la arquitecturas)
Score core;       

int start() {                
    CmodeProtected modeProtected;   
    maps.kstack = KERNEL_STACK_TOP - 16;
    modeProtected.initGdt();
    modeProtected.initIdt();
    //---Inicializacion del Teclado--------------------
    maps.keyMap.port.regBase = 0x60;
    maps.keyMap.port.regControl = 0x64;     
    core.keyborad.initialize();
    //---Inicializacion del Mouse--------------------        
    maps.mouseMap.port.regBase = 0x60;
    maps.mouseMap.port.regControl = 0x64;
    core.mousePS2.initialize();
    //--Inicializacion del reloj real------------------------
    maps.clockMap.port.regBase = 0x70;
    maps.clockMap.port.regControl = 0x71;
    core.clock.refresh();
    //Inicializacion del Controlador de Interrupciones PIC8259
    maps.picMap.master.regBase = 0x20;
    maps.picMap.master.regControl = 0x21;
    maps.picMap.slave.regBase = 0xA0;
    maps.picMap.slave.regControl = 0xA1;
    x86.pic8259.initialize(); 
    //Inicializacion del Temporizador programable de intervalos PIC8253
    maps.pitMap.port.regBase = 0x40;
    maps.pitMap.port.regControl = 0x43;
    x86.pit8253.initialize();
    maps.ticks = 0;
    //Inicializacion disco IDE
    maps.diskIde.chanel0.regBase = 0x1F0;      //Canal 0
    maps.diskIde.chanel0.regControl = 0x3F6;
    maps.diskIde.chanel1.regBase = 0x170;     //Canal 1
    maps.diskIde.chanel1.regControl = 0x376;  
     //Inicializacion de la paginacion y la memoria virtual
    maps.memoryPagination.directoryPageKernel = KERNEL_PAGE_DIR;
    maps.memoryPagination.kernelPage1 = KERNEL_PAGE1;
    maps.memoryPagination.kernelPage1End = KERNEL_PAGE1_END;
    maps.memoryPagination.ramMaxSize = RAM_MAX_SIZE;
    maps.memoryPagination.ramMaxPage =RAM_MAX_PAGE;
    maps.memoryPagination.pageSize = PAGE_SIZE;
    maps.memoryPagination.ramPhisic = 0x04000000; //64 MByte
    maps.memoryPagination.kernelHeap = KERN_HEAP;
    maps.memoryPagination.kernelEnd = KERNEL_END;
    maps.memoryPagination.kernelHeapEnd = KERN_HEAP_END;
    maps.memoryPagination.kernelPageHeap = KERNEL_PAGE_HEAP;
    maps.memoryPagination.kernelPageHeapEnd = KERNEL_PAGE_HEP_END;
    maps.memoryPagination.kernelMaxPageVirtual = VIRTUAL_PHEAP;
    maps.memoryPagination.userPageHeap = USER_PAGE;
    maps.memoryPagination.userPageHeapEnd = USER_PAGE_END;
    maps.memoryPagination.userStack = USER_STACK;
    x86.virtualMemory.initialize();
   //Se carga la IDT y se activan las interrupciones
    asm("lidtl (%0)" : "=m"(maps.kidtr)); 
    asm("sti");
    //Se Cargar el Registro de Tareas y se Salta al Kernel
    asm("movw $0x38, %ax; ltr %ax");	 
    kernel();    
}