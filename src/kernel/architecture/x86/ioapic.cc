#include "ioapic.h"
#include "x86.h"

extern Sx86 x86;

Cioapic::Cioapic() {
}

void Cioapic::initialized(u32 address) {
    apicBase =  address;
    x86.virtualMemory.addDirectoryPagesKernel((u8*)apicBase , (u8*)apicBase , WM_PG_CACHE_DISABLED);
    ptrApicBase = (Sioapic*)apicBase;
    int i, id, maxintr;
    maxintr = (read(IOAPIC_REG_VER) >> 16) & 0xFF;
    id = read(IOAPIC_REG_ID) >> 24;
    //x86.ioScreen.printf("ID IO APIC [%i] /n",id/*,x86.cpu.smp->getIoPic(0)->apicId*/);
   // if (id != ioapicid)
    //    return -1;

    // Marcar todas las interrupciones activadas por el borde, activo alto, deshabilitado,
    // y no enrutado a ninguna CPU.
    for (i = 0; i < maxintr; i++) {
        write(IOAPIC_REG_TABLE + 2 * i, IOAPIC_INT_DISABLED | (0x20 + i));
        write(IOAPIC_REG_TABLE + 2 * i + 1, 0);
    }
    x86.ioScreen.printf("IO APIC Inicializado.../n");
    enabledInterrupt(0, 0);   //Timer
    enabledInterrupt(1, 0);   //Keyboard PS2
    enabledInterrupt(0xC, 0); //Mosue PS2
}

u32 Cioapic::read(u32 reg) {
    volatile (ptrApicBase->reg = reg);
    return volatile (ptrApicBase->value);
}

void Cioapic::write(u32 reg, u32 value) {
    volatile (ptrApicBase->reg = reg);
    volatile (ptrApicBase->value = value);
}

void Cioapic::enabledInterrupt(u32 idIrq, u32 cpu) {
    // Marcar interrupción activada por borde, activa alta,
    // habilitado y enrutado al cpunum dado,
    // que resulta ser la APIC ID de esa CPU.
    write(IOAPIC_REG_TABLE + 2 * idIrq, 0x20 + idIrq);
    write(IOAPIC_REG_TABLE + 2 * idIrq + 1, cpu << 24);
}
