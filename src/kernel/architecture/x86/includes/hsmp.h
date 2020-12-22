/*Copyright (C) 2019  Ramón Mayedo Morales (ramonmayedo@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef HSMP_H
#define HSMP_H
#include "../../../defines/types.h" 

#define PTR_BIOS_EBDA     0x00080000  //Inicio del EBDA del BIOS
#define PTR_BIOS_END_EBDA 0x000FFFFF  //Fin del EBDA del BIOS
#define MP_ENTRY_PROC     0x00        //Entrada a un procesador, tamaño 20 byte
#define MP_ENTRY_IOAPIC   0x02        //Entrada a IO APIC, tamaño 8 byte

//Estructura de puntero flotante MP.
struct SmpFloatingPointStruct {
    u8  signature[4] ;                //Firma _MP_
    u32 tableConfig ; 
    u8  lenght ;                     // En 16 bytes (por ejemplo, 1 = 16 bytes, 2 = 32 bytes) 
    u8  specificationRevision ; 
    u8  checksum;                    //Este valor debe hacer que todos los bytes de la tabla sean iguales a 0 cuando se agreguen juntos 
    u8  defaulConfig ;    
    u32 features;                    // y se debe cargar una configuración predeterminada en su lugar uint32_t features ; // Si el bit 7 es entonces el IMCR está presente y se está utilizando el modo PIC, de lo contrario                                        // el modo de cable virtual es; todos los demás bits están reservados 
}__attribute__ ((packed));

//Tabla de configuración, señalada por la estructura de puntero flotante
struct SmpConfigTable {
    u32   signature;         // "PCMP" 
    u16  length; 
    u8   specificationRevision; 
    u8   checksum;              // Una vez más, el byte debe ser que todos los bytes de la tabla se sumen a 0 
    u8   oemId[8]; 
    u8   vendorId[12]; 
    u32  oemTable ; 
    u16  oemtableSize; 
    u16  entryCount ;             // Este valor representa cuántas entradas están siguiendo esta tabla 
    u32 lapicAddress ;            // Esta es la dirección asignada de memoria de los APIC locales 
    u16 extendedTableLength ; 
    u8  extendedTableChecksum ; 
}__attribute__ ((packed));

//
struct SentryProcessor { 
    u8  type;               // Siempre 0 
    u8  localApicId; 
    u8  localApicVersion; 
    u8  flags;              // Si el bit 0 , entonces se debe ignorar el procesador, Si se establece el bit 1, entonces el procesador es el procesador bootstrap                    
    u32 signature; 
    u32 featureFlags ; 
    u64 reserved; 
}__attribute__ ((packed));

//
struct SentryIoApic { 
    u8 type ;              // Siempre 2 
    u8 apicId ; 
    u8 version ; 
    u8 flags ;             // Si se establece el bit 0, la entrada debe ignorarse 
    u32 address ;          // La dirección asignada de memoria del IO APIC es memoria 
}__attribute__ ((packed));


#endif

