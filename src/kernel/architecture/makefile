#variables
ARCH=x86
KERNEL=kernel.o

#para la configuracion del bootloader
include ./$(ARCH)/configmakefile 
include ./$(ARCH)/makefile 


ensamblar : $(KERNEL)

$(KERNEL): $(OBJASM)
$(OBJASM): $(FILEASM)
$(FILEASM):

%.o: %.asm
	$(ASM) $< -o  $@

