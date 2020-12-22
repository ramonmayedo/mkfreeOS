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

#include "apic.h"
#include "includes/hapic.h"
#include "x86.h"

extern Sx86 x86;
extern Score core;
extern Smaps maps;
extern u32 asm_interrupt_apic_spurious;
extern u32 asm_bootCPUs;
extern u32 c_bootCpus;

Clapic::Clapic() {

}

int Clapic::initialized() {
    apicMemoryBase = (u8*) getapicMemoryBase();
    x86.virtualMemory.addDirectoryPagesKernel((u8*) apicMemoryBase, (u8*) apicMemoryBase, WM_PG_CACHE_DISABLED);
    x86.modeProtected.addHandlerInterrupt((IRQ_APIC_SPURIOUS), asm_interrupt_apic_spurious);
    writeApic(REG_LAPIC_VIR, 0x20 + IRQ_APIC_SPURIOUS | ENABLE);
    writeApic(REG_LAPIC_DRC, 0x0B);
    writeApic(REG_LAPIC_LVT, 0x20000 | 0x20);
    writeApic(REG_LAPIC_RRI, 10000000);
    writeApic(REG_LAPIC_LINT0, 0x00010000);
    writeApic(REG_LAPIC_LINT1, 0x00010000);
    if (((readApic(REG_LAPIC_VER) >> 16) & 0xFF) >= 4)
        writeApic(REG_LAPIC_CLVT, 0x00010000);

    writeApic(REG_LAPIC_ELVT, IRQ_APIC_SPURIOUS + 1);
    writeApic(REG_LAPIC_ESR, 0);
    writeApic(REG_LAPIC_ESR, 0);

    writeApic(REG_LAPIC_EOI, 0);
    writeApic(REG_LAPIC_ICRH, 0);

    writeApic(REG_LAPIC_ICRL, BCAST | INIT | LEVEL);
    while (readApic(REG_LAPIC_ICRL) & DELIVS)
        writeApic(REG_LAPIC_TPR, 0);
    x86.ioScreen.printf("LAPIC Inicializado.... /n");
    writeApic(REG_LAPIC_EOI, 0);
}

u32 Clapic::getapicMemoryBase() {
    u32 low, hight;
    x86.cpu.readMSR(low, hight, APIC_BASE_MSR);
    return (low & 0xfffff000);
}

void Clapic::setapicMemoryBase(u32 base) {
    u32 hight = 0;
    u32 low = base;
    x86.cpu.writeMSR(low, hight, APIC_BASE_MSR);
}

u32 Clapic::readApic(u32 index) {
    return volatile (*(u32*)&(apicMemoryBase[index]));
}

void Clapic::writeApic(u32 index, u32 value) {
   volatile (*(u32*)&apicMemoryBase[index] = value);
}

int Clapic::getLapicId() {
    return readApic(ID) >> 24;
}

void Clapic::eoi() {
    writeApic(REG_LAPIC_EOI, 0);
}

bool Clapic::initializedCPUs(Scpus *cpu) {
    core.memory.memcpy((char*) 0x1000, (char*) &asm_bootCPUs, 512); //Codigo de trampolin para la cpu
    SptrBootCpusData *ptrBootCpus = (SptrBootCpusData*) (0x1000 + 492);
    ptrBootCpus->bootCpusCode = (u32) & c_bootCpus; //Codigo de salto al kernel
    ptrBootCpus->bootCPUmagicNumber = 0x0; //Numero magico CPU inicializada
    ptrBootCpus->kgdt = cpu->gdtr; //Tabla de descriptores globales
    ptrBootCpus->directoryPages = VM_KERNEL_PAGE_DIR; //Directorio de paginas
    ptrBootCpus->stackCpu = cpu->kstack; //Pila pila para esta CPU
    u32 cpuId = cpu->entryProcessor->localApicId;

    x86.port.outb(CMOS_PORT, 0xF);
    x86.port.outb(CMOS_PORT + 1, 0x0A);
    
    writeApic(REG_LAPIC_ICRH, cpuId << 24);
    writeApic(REG_LAPIC_ICRL, INIT | LEVEL | ASSERT);
    delayus(10);
    writeApic(REG_LAPIC_ICRL, INIT | LEVEL);

    delayus(10000);
    for (int i = 0; i < 2; i++) {
        writeApic(REG_LAPIC_ICRH, cpuId << 24);
        writeApic(REG_LAPIC_ICRL, STARTUP | (0x1000 >> 12));
        delayus(200);
    }

    maps.ticksKernelDelay = 2; //2 ms en espera de que inicie
    while (ptrBootCpus->bootCPUmagicNumber != 0x0507 && maps.ticksKernelDelay);

    if (ptrBootCpus->bootCPUmagicNumber == 0x0507)
        return true;

    x86.ioScreen.printf("CPU %i NO inizializada... /n", cpuId);
    return false;
}

void Clapic::delayus(u32 delay) {
    for (int i = 0; i < delay; i++)
        x86.port.inb(0x80);
}