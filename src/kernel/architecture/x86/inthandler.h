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

#ifndef INTHANDLER_H
#define INTHANDLER_H
#include "ioapic.h"
#include "pic8259.h"
#include "apic.h"

enum EdeviceInterruptActive {
    intPic8259, intIoApic
};

class CintHandler {
public:
    CintHandler();
    void initialized();
    int enabledInterrupt(u8 index, u32 handler);
    void eoi();
    bool initializedCPUs(Scpus *cpu);
    int getLapicId();
private:
    EdeviceInterruptActive deviceInterruptActive;
    Cpic8259 pic8259;
    Cioapic ioapic;
    Clapic lapic;
};

#endif

