#variables
PROG=notepad.o
PROGBIN = notepad.elf
PATH = ../../../library/includes
LIBRARY = ../../gui.lib ../../../library/library.lib

LD=C:\gcc\bin\ld
LDFLAG= -T linker.ld
SC=C:\gcc\bin\g++
FLAG= -ffreestanding -nostdlib -nostdinc -I $(PATH)  -I uses
OBJDUMP=objdump
OBJCOPY = objcopy
OBJCOPYFLAG = -O elf32-i386


OBJ := main.o $(LIBRARY)
	
ensamblar : $(PROG)

$(PROG):$(OBJ)
	 $(LD) $(LDFLAG) -o $@ $^

%.o: %.cc
	 $(SC) $(FLAG) -c $< -o  $@

%.o: %.asm
	 $(ASM) $(ASMFLAG) $< -o  $@
	
cpybinarioELF:
	$(OBJCOPY) $(OBJCOPYFLAG) $(PROG) $(PROGBIN)

showobj:
	$(OBJDUMP) -D $(PROG)