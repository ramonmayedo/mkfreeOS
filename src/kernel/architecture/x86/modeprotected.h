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

#ifndef MODEPROTECTED_H
#define MODEPROTECTED_H
#include "includes/hcpu.h"
#include "includes/hmodeprotected.h"

class CmodeProtected {
public:
    CmodeProtected();
    void initGdtDescriptor(u32, u32, u8, u8, struct SgdtDescriptor *); //incializa un descriptor global
    void initIdtDescriptor(u16, u32, u16, struct SidtDescriptor *); //inicializa un descriptor de interrupciones
    void initGdt(Scpus* cpu);
    void initIdt(void);
    void addHandlerInterrupt(u8 index, u32 handler);
    void initGDTCpus(Scpus *cpu);
private:
    SidtDescriptor kidt[MP_IDTSIZE]; //Tabla para los descriptores de interrupcion IDT
    Sidtr kidtr; //Registro IDTR
    Sgdtr kgdtr; //Registro GDTR
};

#endif

