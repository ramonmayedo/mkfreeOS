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
#include "ioapic.h"


//Mapa de memoria del kernel se almacena toda la informacion respecto a la arquitectura
Smaps maps;
//Clases globales para uso del kernel (Para arquitectura x86, 32bit)
Sx86 x86;
//Clases globales para uso del kernel (Para todas la arquitecturas)
Score core;       

int start() {
    x86.cpu.initialized();           //Se incializa la CPU, modo protegido
    x86.virtualMemory.initialize();  //Se incia la memoria virtual  
    x86.pit8253.initialize();        //Inicializacion del Temporizador programable de intervalos PIT8253
    core.graphics.installDevice();   //Se monta el dispositivo de video
    x86.ioScreen.initialize();       //Se monta el disposivo de entrada salida al display
    x86.ioScreen.printf("CPU vendor: %s /n",x86.cpu.getcpuVendor());
    x86.cpu.smp.initialized();       //Se incializan los multiples procesadores si hay
    core.keyborad.initialize();      //Inicializacion del Teclado
    core.mousePS2.initialize();      //Inicializacion del Mouse  
    core.clock.refresh();            //Inicializacion del reloj real CMOS
    core.adminProcess.initialize();  //Seinicializa el administrador de Procesos
    x86.intHandler.initialized();    //Se inicializa el control de interrupciones
    x86.cpu.initializedCpus();       //Se inicializan todas la CPU, es necesario despues de activar las interrupciones
    core.cacheDisk.initialize();     //Se inicializa la cache de disco
    core.devices.installDevices();   //Se montan los dispositivos de almacenamiento
    core.diskPartition.initialize(); //Se inicializan las particiones   
    asm("movw $0x38, %ax; ltr %ax"); //Se Cargar el Registro de Tareas y se Salta al Kernel
    kernel();
}