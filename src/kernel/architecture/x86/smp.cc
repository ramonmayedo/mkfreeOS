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

#include "smp.h"
#include "x86.h"

extern Sx86 x86;
extern Score core;

Csmp::Csmp() {
   
}

void Csmp::initialized() {
    countIoApic = 0;
    countProcessors = 0;
    if (getmpFloatingPointStruct()) {
        SmpConfigTable *ptrmpConfigTable = (SmpConfigTable*) mpFloatingPointStruct.tableConfig;
        if (ptrmpConfigTable->signature == 0x504D4350 && checksum((char*) ptrmpConfigTable, sizeof (ptrmpConfigTable->length)) == 0) {
            core.memory.memcpy((char*) &mpConfigTable, (char*) mpFloatingPointStruct.tableConfig, sizeof (SmpConfigTable));
            x86.ioScreen.printf("CPU OEM ID : %s /n", ptrmpConfigTable->oemId);
            ptrEntry = (char*) (mpFloatingPointStruct.tableConfig + sizeof (SmpConfigTable) + 1);
            enumProcessorAndIoApic();
        }
    }
}

int Csmp::getmpFloatingPointStruct() {
    for (u32 i = PTR_BIOS_EBDA; i < PTR_BIOS_END_EBDA; i += 16) { //Se revisa en la zona extendida del bios en busac de la entrada _MP_ alineada en 16
        if (*(u32*) i == 0x5F504D5F) { //Si se encuentra
            if (checksum((char*) i, sizeof (SmpFloatingPointStruct))) { //Se realiza la suma de comprobacion
                core.memory.memcpy((char*) &mpFloatingPointStruct, (char*) i, sizeof (SmpFloatingPointStruct));
                return 1;
            }
        }
    }
    return 0;
}

bool Csmp::checksum(char *ptrTable, u32 size) {
    char sum = 0;
    for (int i = 0; i < size; i++)
        sum += ptrTable[i];
    return (sum == 0);
}

int Csmp::enumProcessorAndIoApic() {
    char *auxPtrEntry = ptrEntry;
    for (int i = 0; i < mpConfigTable.entryCount; i++) {
        if (auxPtrEntry[0] == MP_ENTRY_PROC) {
            if (((SentryProcessor*) auxPtrEntry)->flags & 0x1) {
                core.memory.memcpy((char*) &entryProcessors[countProcessors], (char*) auxPtrEntry, sizeof (SentryProcessor));
                if (((SentryProcessor*) auxPtrEntry)->flags & 0x2){
                    BSProcessor = &entryProcessors[countProcessors];
                    x86.cpu[0]->entryProcessor = &entryProcessors[countProcessors];
                }                    
                countProcessors++; 
            }
            auxPtrEntry += sizeof (SentryProcessor);
        } else if (auxPtrEntry[0] == MP_ENTRY_IOAPIC) {
            if (((SentryIoApic*) auxPtrEntry)->flags & 0x1) {
                core.memory.memcpy((char*) &entryIoApic[countIoApic], (char*) auxPtrEntry, sizeof (SentryIoApic));
                countIoApic++;
            }
            auxPtrEntry += sizeof (SentryIoApic);
        } else
            auxPtrEntry += sizeof (SentryIoApic);
    }
    x86.ioScreen.printf("Cantidad de Procesadores Detectados: [%i] /n", countProcessors);
    x86.ioScreen.printf("Cantidad de IO APIC Detectados: [%i] /n", countIoApic);
}

int Csmp::getcountIoApic() {
    return countIoApic;
}

int Csmp::getcountProcessador() {
    return countProcessors;
}

 SentryIoApic * Csmp::getIoPic(int index){
    if (index > countIoApic)
        return &entryIoApic[0];
    else
        return &entryIoApic[index];
}
 
SentryProcessor* Csmp::getProcessador(int index) {
    if (index > countProcessors)
        return &entryProcessors[0];
    else
        return &entryProcessors[index];
}
