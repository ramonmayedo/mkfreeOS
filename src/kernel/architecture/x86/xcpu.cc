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

#include "xcpu.h"
#include "x86.h"
extern Smaps maps;
extern Sx86 x86;
Cxcpu::Cxcpu() {
}
//CPU BP

void Cxcpu::initialized() {
    getCPUId();
    cpus[0].initialized = true;
    cpus[0].run = 0;
    cpus[0].kstack = 0x9FFF0;
    x86.modeProtected.initGdt(&cpus[0]);
    countCpus = 1;
}

void Cxcpu::getCPUId() {
//La  cadena de identificación del proveedor se devuelve en EBX, ECX, EDX 
//Ejemplo EBX = 'u' 'n' 'e' 'G', EDX = 'I' 'e' 'n' 'i', ECX = 'l' 'e' 't' 'n' 
//Es necesario informarle al compilador los cambios en los registros ebx,ecx,edx,eax y se hace leyendo todos
    asm volatile("cpuid"
                : "=a"((u32*)cpuVendor[0]), "=b"((u32*)cpuVendor[0]),
                "=d"((u32*)cpuVendor[1]), "=c"((u32*)cpuVendor[2])
                : "a"(0));
    cpuVendor[12]  = 0x0;
    int a,b;
    asm volatile("cpuid"
                :"=a"(a), "=b"(b),
                "=d"(cpuFeaturesEDX), "=c"(cpuFeaturesECX)
                : "a"(1));
}

u8* Cxcpu::getcpuVendor() {
    return (u8*)cpuVendor;
}

void Cxcpu::readMSR(u32 &low, u32 &high, u32 msr) {
    asm volatile("rdmsr"
                : "=a"(low), "=d"(high)
                : "c"(msr));
}

void Cxcpu::writeMSR(u32 low, u32 high, u32 msr) {
    asm volatile("wrmsr" : "=b"(low) : "a"(low), "d"(high), "c"(msr));
}

SfeaturesECX Cxcpu::getcpuFeaturesECX() {
    return cpuFeaturesECX;
}

SfeaturesEDX Cxcpu::getcpuFeaturesEDX() {
    return cpuFeaturesEDX;
}

Scpus* Cxcpu::operator[](int index) {
    if (index >= 10)
        return &cpus[9];
    return &cpus[index];
}

Scpus* Cxcpu::getCpu(int lapicId) {
    for (int i = 0; i < countCpus; i++)
        if (cpus[i].entryProcessor->localApicId == lapicId)
            return &cpus[i];
    return 0;
}

void Cxcpu::initializedCpus() {
    int countPro = smp.getcountProcessador();
    if (countPro > 1) {
        for (int i = 1; i < countPro; i++) {
            cpus[countCpus].entryProcessor = smp.getProcessador(i);
            x86.modeProtected.initGDTCpus(&cpus[countCpus]);
            if (x86.intHandler.initializedCPUs(&cpus[countCpus])) {
                cpus[countCpus].initialized = true;
                cpus[countCpus].run = 0;
                countCpus++;
            }
        }
    }
}