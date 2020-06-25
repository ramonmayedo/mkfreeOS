#variables
PROG=gui.lib
PATH=..\library\includes
SC=C:\gcc\bin\g++
FLAG= -ffreestanding -nostdlib -nostdinc -fno-rtti -I $(PATH) -I basics
ASM=C:\nasm\nasm
ASMFLAG = -f elf 
OBJDUMP=objdump

OBJ := $(OBJ) basics/control.o basics/point.o basics/size.o basics/cstring.o\
	      basics/rect.o basics/color.o basics/list.o basics/object.o \
	      basics/frame.o basics/graphicsbitmap.o basics/fonts/lucidaconsole.o \
	      basics/font.o components/Gbutton.o components/Gpanel.o \
	      components/Gcursor.o components/Gwindows.o components/GWbartitle.o \
	      components/Glabel.o components/GlistBox.o basics/elapseTime.o \
	      components/GscrollBar.o basics/kgraphics.o basics/vstring.o \
	      components/GtextEdit.o basics/slines.o components/GlineEdit.o \
	      basics/columns.o components/GlistView.o components/GcolumnButton.o\
	      systemWindows/Gclient.o components/GstatusBar.o components/GtoolBar.o
	
ensamblar : $(PROG)

$(PROG):$(OBJ)
	 ar rcs $@ $(OBJ)
%.o: %.cc
	 $(SC) $(FLAG) -c $< -o  $@

%.o: %.asm
	 $(ASM) $(ASMFLAG) $< -o  $@
	
showobj:
	$(OBJDUMP) -D $(PROG)
	
