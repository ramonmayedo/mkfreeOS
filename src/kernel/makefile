#variables
ARCH=x86
KERNEL=kernel.o
KERBIN = kernel.bin
SDKDIR=../
#para la configuracion del bootloader
include ./architecture/$(ARCH)/configmakefile 
include ./architecture/$(ARCH)/makefile 
include ./core/makefile
include ./core/compiler/makefile
include ./core/filesystem/makefile
include ./drivers/makefile
include ./uses/makefile
include ./files/makefile
#objetivo es obtener compilar , dependencia es obtener $(KERNEL) , comandos es obtener .o
ensamblar : $(KERNEL)

$(KERNEL):$(OBJ)
	$(LD) $(LDFLAG) -o $@ $^
	$(OBJCOPY) $(OBJCOPYFLAG) $(KERNEL) $(KERBIN)

%.o: %.cc
	$(SC) $(FLAG) -c $< -o  $@
	 
%.o: %.asm
	$(ASM) $(ASMFLAG) $< -o  $@
	
cpybinario:
	$(OBJCOPY) $(OBJCOPYFLAG) $(KERNEL) $(KERBIN)

showobj:
	$(OBJDUMP) -D $(KERNEL)

help:
	@echo "Ayuda para compilar el Kernel de MKFREE"
	@echo "Comandos Actuales |compiler|cpybinario|showobj|"
	
#ayuda-----------
#-----sintaxis para make--------  
# objetivo: dependencia
# comandos
#$(KERNEL):$(OBJ) el objetivo es obtner $(KERNEL) y la dependencia $(OBJ) 
#el comando $(LD) $(LDFLAG) -o $@ $^ hace el fichero objetivo
#$@ significa que sustituye por el nombre del objetivo de la presente regla
#$^ Se sustituye por una lista separada por espacios de cada una de las dependencias de la presente regla
#ejemplo 
#    ld -e .text -Ttext 0x0000 -o kernel.o xxxx.o xxx.o ....
#
#%.o: %.cc   regla patro que todo objetivo que sea un .o y tenga dependencia .cc
#	$(SC) $(FLAG) -c $< -o  $@
#$<  Se sustituye por la primera dependencia de la presente regla
#$@  Se sustituye por el nombre del objetivo de la presente regla
#ejemplo
#   g++ -c xxx.c xxxx.o