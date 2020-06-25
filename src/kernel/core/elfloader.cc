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

#include "elfloader.h"
#include "../architecture/x86/x86.h"
extern Sx86 x86;
extern Smaps maps;

CelfLoader::CelfLoader() {
}

int CelfLoader::load(u8 *afile, SprocessX86 *aprocessX86) {
    Self32Ehdr *elf32Ehdr = (Self32Ehdr*) afile;
    Self32Phdr *elf32Phdr = (Self32Phdr*) (afile + elf32Ehdr->phoff);
    Self32Scdr *elf32Scdr = (Self32Scdr*) (afile + elf32Ehdr->shoff);
    u32 virtualAddrBegin, virtualAddrEnd;

    for (int i = 0; i < elf32Ehdr->phnum; i++, elf32Phdr++) {
        if (elf32Phdr->type == PT_LOAD) {
            virtualAddrBegin = elf32Phdr->vaddr;
            virtualAddrEnd = elf32Phdr->vaddr + elf32Phdr->memsz;

            if (maps.memoryPagination.userPageHeap > virtualAddrBegin) return 0;
            if (maps.memoryPagination.userPageHeapEnd < virtualAddrEnd) return 0;

            if (elf32Phdr->flags == PF_X + PF_R) {
                aprocessX86->exec.begin = (u8*) virtualAddrBegin;
                aprocessX86->exec.end = (u8*) virtualAddrEnd;
                aprocessX86->exec.size = elf32Phdr->memsz;
                aprocessX86->exec.offsetFile = (u32) afile + elf32Phdr->offset;
            }
            if (elf32Phdr->flags == PF_W + PF_R) {
                aprocessX86->bss.begin = (u8*) virtualAddrBegin;
                aprocessX86->bss.end = (u8*) virtualAddrEnd;
                aprocessX86->bss.size = elf32Phdr->memsz;
                aprocessX86->bss.offsetFile = (u32) afile + elf32Phdr->offset;
            }
        }
    }
    return 1;
}

int CelfLoader::isValidElf(u8 *afile) {
    Self32Ehdr *elf32Ehdr = (Self32Ehdr *) afile;
    if (elf32Ehdr->ident[0] == 0x7f && elf32Ehdr->ident[1] == 'E'
            && elf32Ehdr->ident[2] == 'L' && elf32Ehdr->ident[3] == 'F')
        return 0;
    else
        return 1;
}