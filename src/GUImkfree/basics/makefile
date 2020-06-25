#variables
PROG=basic.lib
PATH=..\library\includes
SC=C:\gcc\bin\g++
FLAG= -ffreestanding -nostdlib -nostdinc -fno-rtti -I $(PATH) -I basics
ASM=C:\nasm\nasm
ASMFLAG = -f elf 
OBJDUMP=objdump

OBJ := $(OBJ) control.o point.o size.o cstring.o\
	      rect.o color.o list.o object.o \
	      frame.o graphicsbitmap.o fonts/lucidaconsole.o \
	      font.o  elapseTime.o kgraphics.o vstring.o \
	      slines.o  columns.o
	
ensamblar : $(PROG)

$(PROG):$(OBJ)
	 ar rcs $@ $(OBJ)
%.o: %.cc
	 $(SC) $(FLAG) -c $< -o  $@

%.o: %.asm
	 $(ASM) $(ASMFLAG) $< -o  $@
	
showobj:
	$(OBJDUMP) -D $(PROG)
	
