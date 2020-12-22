#ifndef IOAPIC_H
#define IOAPIC_H
#include "../../defines/types.h"

#define IOAPIC 0xFEC00000 // Dirección física predeterminada de IO APIC

#define IOAPIC_REG_ID    0x00        //Índice de registro: ID
#define IOAPIC_REG_VER   0x01        //Índice de registro: versión
#define IOAPIC_REG_TABLE 0x10        //Base de la tabla de redireccionamiento

// La tabla de redireccionamiento comienza en REG_TABLE y usa
// dos registros para configurar cada interrupción.
// El primer registro (bajo) en un par contiene bits de configuración.
// El segundo registro (alto) contiene una máscara de bits que indica qué
// Las CPU pueden servir esa interrupción.
#define IOAPIC_INT_DISABLED   0x00010000    // Interrupción deshabilitada
#define IOAPIC_INT_LEVEL      0x00008000    // Activado por nivel (vs edge-)
#define IOAPIC_INT_ACTIVELOW  0x00002000    // Activo bajo (frente a alto)
#define IOAPIC_INT_LOGICAL    0x00000800    // El destino es la identificación de la CPU (vs APIC ID)

struct Sioapic {
    u32 reg;
    u32 pad[3];
    u32 value;
}__attribute__ ((packed));;

class Cioapic {
public:
    Cioapic();
    void initialized(u32 address);
    void write(u32 reg,u32 value);
    u32  read(u32 reg);
    void enabledInterrupt(u32 idIrq, u32 cpu);
private:
    volatile Sioapic *ptrApicBase;
    u32 apicBase;
};

#endif

