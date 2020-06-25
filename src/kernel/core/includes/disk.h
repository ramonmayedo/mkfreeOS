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

#ifndef DISK_H
#define DISK_H
#include "../../defines/types.h"

/*   Registro de Arranque Maestro
 *   |0x0000| 	Área local social
 *   |0x01B8| 	4 bytes; firma del disco (opcional)
 *   |0x01BC| 	2 bytes; normalmente 0x0000
 *   |0x01BE| 	Tabla de particiones primarias, cada entrada es de 16 bytes
 *   |0x01FE| 	2 bytes; firma del MBR (0x55AA)  
 * 
 * 
 *   Tabla de Particiones Offset Sector 0 dir 0x01BE
 *   | Particion 0 0x01BE  16Byte de longitud|
 *   | Particion 1 0x01CE  16Byte de longitud|
 *   | Particion 2 0x01DE  16Byte de longitud|
 *   | Particion 3 0x01EE  16Byte de longitud| 
 *   
 *    Formato de la Particion
 *    | byte 0| (1) Estado si 0x80 es arranclable
 *    | Byte 1| (3) Cominenzo de la poarticion Cilindro,Cabezal y Sector  CHS
 *    | Byte 4| (1) Tipo de Particion
 *    | Byte 5| (3) Fin de la particion Cilindro, Cabezal y Sector (CHS)
 *    | Byte 8| (4) Logical block address del primer sector de la partición 
 *    | Byte C| (4) Longitud de la partición, en sectores  
 * 
 */


#define PARTITIONBASE 0x01BE

struct Spartition {
    u8 flagEstate; //Estado 0x80 particion Arrancable
    u8 startHead;
    u16 startSector : 6;
    u16 startCylinder : 10;
    u8 typePartition;
    u8 endHead;
    u16 endSector : 6;
    u16 endCylinder : 10;
    u32 startLBA;
    u32 sizePartiton;
} __attribute__((packed));

struct SbootMasterRecord {
    Spartition partiton0;
    Spartition partiton1;
    Spartition partiton2;
    Spartition partiton3;
} __attribute__((packed));


#endif