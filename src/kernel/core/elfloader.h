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

#ifndef ELFLOADER_H
#define ELFLOADER_H
#include "../defines/types.h"
#include "includes/hprocess.h"

/* Self32Phdr type */
#define	PT_NULL             0
#define	PT_LOAD             1
#define	PT_DYNAMIC          2
#define	PT_INTERP           3
#define	PT_NOTE             4
#define	PT_SHLIB            5
#define	PT_PHDR             6
#define	PT_LOPROC  0x70000000
#define	PT_HIPROC  0x7fffffff

/* Self32Phdr flags */
#define PF_X	0x1
#define PF_W	0x2
#define PF_R	0x4

struct Self32Ehdr {
      u8  ident[16];	        // identificacion del ELF
      u16 type;		        // tipo de archivo ELF
      u16 machine;		// arquitectura
      u32 version;		//version
      u32 entry;		//punto de entrada
      u32 phoff;		/* program header table offset */
      u32 shoff;		/* section header table offset */
      u32 flags;		/* various flags */
      u16 ehsize;		/* ELF header (this) size */
      u16 phentsize;	      /* program header table entry size */
      u16 phnum;		/* number of entries */
      u16 shentsize;	/* section header table entry size */
      u16 shnum;		/* number of entries */
      u16 shstrndx;		/* index of the section name string table */
};

struct Self32Phdr{
	u32 type;		/* type of segment */
	u32 offset;
	u32 vaddr;
	u32 paddr;
	u32 filesz;
	u32 memsz;
	u32 flags;
	u32 align;
};

struct Self32Scdr{
	u32	name;
	u32	type;
	u32	flags;
	u32	address;
	u32	offset;
	u32	size;
	u32	link;
	u32	info;
	u32	addralign;
	u32	entsize;
};



class CelfLoader {
public:
    CelfLoader();
    int isValidElf(u8 *afile);
    int load(u8 *afile,SprocessX86 *aprocessX86);
private:

};

#endif

