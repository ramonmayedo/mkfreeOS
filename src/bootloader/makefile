ARCH = x86
LOADER = $(ARCH)/loader.b
BIN = $(ARCH)/loader.bin

include ./$(ARCH)/configmakefile 
include ./$(ARCH)/makefile 

compiler : $(LOADER)  $(OBJASM)
	
$(LOADER) : $(OBJ)
	$(LD) $(LDFLAG) -o $@ $^
	$(OBJCOPY) $(OBJCOPYFLAG) $(LOADER) $(BIN)

%.o: %.asm
	$(ASM) $< -o  $@

%.o: %.c
	$(SC) $(FLAG) -c $< -o  $@

showobj:
	$(OBJDUMP) -D $(LOADER)