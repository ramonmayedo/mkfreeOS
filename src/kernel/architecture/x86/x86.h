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

#ifndef X86_H
#define X86_H
//-----------includes del x86------------
#include"inthandler.h"
#include "xcpu.h"
#include "pit8253.h"
#include "ioscreen.h"
#include "virtualmemory.h"
#include "../../vmemory/malloc.h"
#include "io.h"
#include "modeprotected.h"
#include "architecture.h"
//------------includes del core----------
#include "../../drivers/clock.h"
#include "../../drivers/keyborad.h"
#include "../../drivers/diskide.h"
#include "../../core/diskpartition.h"
#include "../../core/filesystem.h"
#include "../../uses/string.h"
#include "../../uses/conversion.h"
#include "../../core/elfloader.h"
#include "../../core/systemcall.h"
#include "../../uses/list.h"
#include "../../drivers/vga.h"
#include "../../core/graphics.h"
#include "../../core/adminProcess.h"
#include "../../uses/memory.h"
#include "../../core/exclusion.h"
#include "../../core/cacheDisk.h"
#include "../../core/pioscreen.h"
#include "../../drivers/cdide.h"
#include "../../core/devices.h"
#include "../../drivers/mouse.h"
#include "../../drivers/pci.h"
//--------------Espacio del Kernel-------------------------------------------------------------
#define	KERNEL_BASE          0x00100000  //Direccion donde incia el kernel 1MB Byte dipsonible    
#define	KERNEL_DATA          0x00200000  //Varibles del kernel zona de memoria reservada 1 MByte 
#define KERNEL_STACK_TOP     0x00100000  //Tope de la pila del kernel

struct Smaps {
    SregX86Info statusX86;                   //Estado del rpocesador ante una falla
    SclockMap clockMap;                  //Info del Reloj RTC
    SmemoryPagination memoryPagination;  //Info de la paginacion
    u32 ticks;                           //tick de reloj para el cambio de tarea
    u32 ramUsed;
    int ticksKernelDelay;                //Para demoras de hardware
};

struct Sx86 {
    Cxcpu cpu;                     //informacion de la cpu
    CmodeProtected modeProtected;  //Acceso al modo protegido idt y ldt
    CioScreen ioScreen;            //acceso al termina de pantalla
    Cio port;                      //acceso a los puertos de entrada salida
    CintHandler intHandler;        //acceso al controlador de interrupciones
    Cpit8253 pit8253;              //acceso al Timer 
    CvirtualMemory virtualMemory;  //acceso a la memoria virtual
    Cmalloc malloc;                //acceso a rutinas de malloc
    Carchitecture architecture;    //acceso a rutinas dependientes del procesador
};

struct Score{
    Ckeyboard keyborad;            //acceso al teclado
    CmausePS2 mousePS2;            //acceso al mause
    Cclock clock;                  //acceso al reloj
    Cpci pci;                      //Accesos a los dispositivos PCI
    Cdevices devices;              //Acceso a los dispositivos de almacenamiento
    Cdiskide diskide;              //acceso al disco 
    Ccdide cdide;                  //accceso CD
    CdiskPartition diskPartition;  //Sistema de particiones
    CfileSystem fileSystem;        //Sistema de Archivos
    Cstring     string;            //Manejo de string
    Cconversion conversion;        //Conversiones
    CelfLoader  elfLoader;         //Manejador de archivos ELF
    CsystemCall systemCall;        //LLamadas al sistema
    Cgraphics graphics;            //LLamadas al sistema de salida en pantalla
    CadminProcess adminProcess;    //Administracion de procesos
    Cmemory memory;                //Manejo de memoria
    Cexclusion exclusion;          //Semaforos
    CcacheDisk cacheDisk;          //Cache para discos
    Cpioscreen pioscreen;          //Conecta un proceso al terminal del kernel
};

extern "C" {
        int main();  
};

#endif