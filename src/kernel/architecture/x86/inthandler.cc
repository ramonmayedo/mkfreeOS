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

#include "inthandler.h"
#include "x86.h"

extern Sx86 x86;
extern Score core;
extern Smaps maps;

CintHandler::CintHandler() {
}

int CintHandler::enabledInterrupt(u8 index, u32 handler) {
    if (deviceInterruptActive == intIoApic)
        ioapic.enabledInterrupt(index, handler);
}

void CintHandler::eoi() {
    if (deviceInterruptActive == intPic8259)
        pic8259.eoi();
    else if (deviceInterruptActive == intIoApic)
        lapic.eoi();
}

void CintHandler::initialized() {
    x86.modeProtected.initIdt();
    pic8259.initialize();
    deviceInterruptActive = intPic8259;
    if (x86.cpu.getcpuFeaturesEDX().APIC) { //Si soporta APIC se cambia de manera automatica
        pic8259.diseabled();
        lapic.initialized();
        ioapic.initialized(0xFEC00000);
        deviceInterruptActive = intIoApic;
    }
    asm("sti");
}

bool CintHandler::initializedCPUs(Scpus *cpu) {
    return lapic.initializedCPUs(cpu);
}

int CintHandler::getLapicId() {
    return lapic.getLapicId();
}
