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

#ifndef HPROCESS_H
#define HPROCESS_H
#include "../../architecture/x86/includes/reg.h"
#include "../file.h"

#define MAX_COMMANDS 30

struct SsegFile {
    u32 offsetFile;
    u8* begin;
    u8* end;
    u32 size;
} __attribute__((packed));

struct SprocessX86 {
    u32 *pageDirectory;
    u32 *pageDirectoryVirtual;
    u32 cr3;
    SsegFile bss;
    SsegFile exec;
    SsegFile heap;
    SsegFile stack;
    u32 stackThread;
} __attribute__((packed));

struct SfilesProcess{
    Cfile *file;
    int id;
} __attribute__((packed));

struct ScommandProcess {
    int pidTrasmisor;
    int command;
    int parameter1;
    int parameter2;
    int parameter3;
} __attribute__((packed));

struct ScontrolTime{
    u32 standarTime;
    int plannedTime;
    u32 beforeConsumedTime;
    u32 filter;
    u32 timeConsumed;
};

#endif

