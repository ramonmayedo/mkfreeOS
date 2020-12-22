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
#ifndef SMP_H
#define SMP_H
#include "includes/hsmp.h"

class Csmp {
public:
    Csmp();
    void initialized();
    int getcountIoApic();
    int getcountProcessador();
    SentryIoApic * getIoPic(int index);
    SentryProcessor *getProcessador(int index);
    void initializedCpus();
private:
    int countProcessors;
    int countIoApic;
    char *ptrEntry;
    SmpFloatingPointStruct mpFloatingPointStruct;
    SmpConfigTable mpConfigTable;
    SentryProcessor entryProcessors[12];
    SentryIoApic entryIoApic[3];
    SentryProcessor *BSProcessor;

    int getmpFloatingPointStruct();
    int enumProcessorAndIoApic();
    bool checksum(char *ptrTable, u32 size);

};

#endif

