typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
void outb(u32 addr, u8 value);
u8 inb(u32 addr);
u16 inw(u32 addr);
int waitDisk();
void initCommand(u32 adevice, u32 ablock, u32 asectorCount);
int readSector(u32 device, u32 ablock, u32 asectorCount, char *abuffer);

int _main(){
    readSector(0,56,220,(char*)0x100000);
    asm("mov $0x000FFF0,%esp");
    asm("ljmp $0x18,$0x100000"); //Se salta a la entrada del codigo del kernel
}
void outb(u32 addr, u8 value) {
    asm("movb %0,%%al" : "=m"(value));
    asm("mov %0,%%dx" : "=m"(addr));
    asm("outb %al, %dx");
}

u8 inb(u32 addr) {
    u8 value;
    asm("mov %0,%%dx" : "=m"(addr));
    asm("inb %dx,%al");
    asm("movb %%al,%0" : "=m"(value));
    return value;
}

u16 inw(u32 addr){
  u16 value; 
  asm("mov %0,%%dx" :"=m"(addr));   
  asm("inw %dx, %ax");
  asm("movw %%ax,%0" :"=m"(value)); 
  return value;   
}

int waitDisk() {
    while (inb(0x3F6) & 0x80);
    return 0;
}

void initCommand(u32 adevice, u32 ablock, u32 asectorCount) {
    //Se espera que el dipositivo 1 este desocupado
    waitDisk();
    //Byte 0
    outb(0x1F0 + 0x1, 0x0);
    //byte 1
    outb(0x1F0 + 0x2, asectorCount);
    //byte 2
    outb(0x1F0 + 0x3, (u8) ablock);
    //byte 3
    outb(0x1F0 + 0x4, (u8) (ablock >> 8));
    //byte 4
    outb(0x1F0 + 0x5, (u8) (ablock >> 16));
    //byte 5
    u8 mask = (u8) ((ablock & 0x0F000000) >> 24) | adevice << 4 | 0xE0;
    outb(0x1F0 + 0x6, mask);

}

int readSector(u32 device, u32 ablock, u32 asectorCount, char *abuffer) {
    initCommand(device, ablock, asectorCount);
    //byte 7 comando = 0x1F0 lectura
    outb((0x1F0 + 0x7), 0x20);
        
    waitDisk();             
     while (!(inb(0x1F0 + 0x7)&0x08));
    u16 auxWord;
    
    for (int i = 0; i < 256 * asectorCount; i++) {
        auxWord = inw(0x1F0);
        abuffer[i * 2] = (u8) auxWord;
        abuffer[i * 2 + 1] = (u8) (auxWord >> 8);
    }
    return asectorCount;
}
