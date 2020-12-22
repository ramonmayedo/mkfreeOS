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

#ifndef XCPU_H
#define XCPU_H
#include "../../defines/types.h"
#include "includes/hcpu.h"
#include "smp.h"

class Cxcpu {
public:
    Cxcpu();
    void initialized();
    u8* getcpuVendor();
    void readMSR(u32 &low, u32 &high, u32 msr);
    void writeMSR(u32 low, u32 high, u32 msr);
    SfeaturesEDX getcpuFeaturesEDX();
    SfeaturesECX getcpuFeaturesECX();
    Scpus * operator[](int index);
    Scpus * getCpu(int lapicId);
    void initializedCpus();
    Csmp smp;
    u32  countCpus;
private:
    u8* cpuVendor[13];           //Nombre del fabricante del procesador
    SfeaturesEDX cpuFeaturesEDX; //Características de la CPU
    SfeaturesECX cpuFeaturesECX; //Características de la CPU solo CPU modernas
    Scpus cpus[10];              //Registros unicos en cada CPU. TSS,GDT,...
protected:
    void getCPUId();
};

#endif

