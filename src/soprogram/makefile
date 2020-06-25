#variables
PROG=system.lib
PATH=includes
SC=C:\gcc\bin\g++
FLAG= -ffreestanding -nostdlib -nostdinc -I $(PATH)
ASM=C:\nasm\nasm
ASMFLAG = -f
OBJDUMP=objdump

OBJ := $(OBJ)  uses/application.o
	
	
ensamblar : $(PROG)

$(PROG):$(OBJ)
	 ar rcs $@ $(OBJ)
%.o: %.cc
	 $(SC) $(FLAG) -c $< -o  $@

%.o: %.asm
	 $(ASM) $(ASMFLAG) $< -o  $@
	
showobj:
	$(OBJDUMP) -D $(PROG)