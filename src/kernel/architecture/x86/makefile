#se compila siempre el archivo x86 primero ya que contiene la entrada a main()
OBJ :=  architecture/$(ARCH)/x86.o $(OBJ) architecture/$(ARCH)/modeprotected.o architecture/$(ARCH)/io.o \
	architecture/$(ARCH)/star.o architecture/$(ARCH)/ioscreen.o architecture/$(ARCH)/pic8259.o \
	architecture/$(ARCH)/intfunction.o architecture/$(ARCH)/virtualmemory.o \
	architecture/$(ARCH)/malloc.o architecture/$(ARCH)/architecture.o architecture/$(ARCH)/pit8253.o \
	architecture/$(ARCH)/sharedmemory.o