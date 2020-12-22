#variables
PROG=gui.lib
PATH=..\library\includes
SC=C:\gcc\bin\g++
FLAG= -ffreestanding -nostdlib -nostdinc -fno-rtti -I $(PATH) -I basics
ASM=C:\nasm\nasm
ASMFLAG = -f elf 
OBJDUMP=objdump

OBJ := $(OBJ) basics/canvasPainter.o basics/gpoint.o basics/gsize.o basics/cstring.o\
	      basics/grect.o basics/gcolor.o basics/list.o basics/object.o \
	      basics/gbitmap.o  \
	      basics/font.o \
	      basics/socketipc.o \
	      basics/vstring.o \
	      basics/slines.o resources/icons.o \
	      basics/columns.o basics/socketipc.o \
	      basics/png.o basics/puff.o \
	      components/GgraphicComponent.o components/Gcomponent.o \
	      components/Gwindows.o client/Gclient.o components/Gbutton.o\
	      components/GscrollBar.o components/GcontrolScrolled.o \
	      components/GlistView.o
	
ensamblar : $(PROG)

$(PROG):$(OBJ)
	 ar rcs $@ $(OBJ)
%.o: %.cpp
	 $(SC) $(FLAG) -c $< -o  $@

%.o: %.cc
	 $(SC) $(FLAG) -c $< -o  $@

%.o: %.asm
	 $(ASM) $(ASMFLAG) $< -o  $@
	
showobj:
	$(OBJDUMP) -D $(PROG)
	
