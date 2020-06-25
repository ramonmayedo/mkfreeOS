#variables
PROG=library.lib
PATH=includes
SC=C:\gcc\bin\g++
FLAG= -ffreestanding -nostdlib -nostdinc -I $(PATH)
ASM=C:\nasm\nasm
ASMFLAG = -f
OBJDUMP=objdump

OBJ := $(OBJ) code/stdio.o code/syscall.o code/malloc.o code/compiler/operator.o \
	code/string.o  code/stdio.o code/unistd.o code/conio.o code/time.o \
	code/conversion.o code/stdlib.o code/memory.o code/dirent.o \
	code/thread.o
	
	
ensamblar : $(PROG)

$(PROG):$(OBJ)
	 ar rcs $@ $(OBJ)
%.o: %.cc
	 $(SC) $(FLAG) -c $< -o  $@

%.o: %.asm
	 $(ASM) $(ASMFLAG) $< -o  $@
	
showobj:
	$(OBJDUMP) -D $(PROG)