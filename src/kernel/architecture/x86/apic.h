#ifndef APIC_H
#define APIC_H
#include "../../defines/types.h"
#include "includes/hapic.h"
#include "includes/hcpu.h"

struct SptrBootCpusData {
    Sgdtr kgdt;
    u32 stackCpu;
    u32 directoryPages;
    u32 bootCpusCode;
    u16 bootCPUmagicNumber;
} __attribute__((packed));

class Clapic {
public:
    Clapic();
    int initialized();
    u32 getapicMemoryBase();
    void setapicMemoryBase(u32 base);
    u32 readApic(u32 index);
    void writeApic(u32 index, u32 value);
    int getLapicId();
    void eoi();
    bool initializedCPUs(Scpus *cpu);
private:
    void delayus(u32 delay);
    volatile u8 *apicMemoryBase;
    volatile SptrIoApic *ptrIoApic;
};

#endif

