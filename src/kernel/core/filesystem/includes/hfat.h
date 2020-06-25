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

#ifndef HFAT_H
#define HFAT_H
#include "../../../defines/types.h"
#include "../../filesystem.h"

#define FAT_32 0xC
//tipos de entradas en la fat
#define FREE        0x00000000      //Cluster libre
#define RESERVED    0x00000001      //Cluster reservado
#define BAD_CLUSTER 0x0FFFFFF7      //Cluster malo
#define USED_S      0x00000002      //Cluster usado 0x00000002-0x0FFFFFF6
#define USED_F      0x0FFFFFF6      //
#define END_CHAIN_S 0x0FFFFFF8      //Fin Chain 0x0FFFFFF8  - 0x0FFFFFFF
#define END_CHAIN_F 0x0FFFFFFF      
//atributos de los archivos
#define ATTR_READ_ONLY      0x01    // (Read-only)
#define ATTR_HIDDEN         0x02    // ATTR_HIDDEN (Hidden)
#define ATTR_SYSTEM         0x04    //(System)
#define ATTR_VOLUME_ID      0x08    //(Volume label)
#define ATTR_DIRECTORY      0x10    //(Directory)
#define ATTR_ARCHIVE        0x20    //(Archive)
#define ATTR_LONG_FILE_NAME 0x0F    //(LFN entry)
//errores ocurridos dentro de la manipulacion de los archivos
#define FAT_FAT_ENTRY_FULL        0x5   //No hay entradas vacias en la FAT
#define FAT_NOT_LABEL_VOLUME      0x7   //No tiene nombre la etiquta
#define FAT_ENTRY_DIR_CORRUPT     0x9   //Entrada corrupta en los directorios
#define FAT_LFN_ENTRY_INVALID     0xA   //tamaño del nombre invalido 1 a 20
//comandos para la manipulacion de losarchivos
#define FAT_RENAME                0x5    //Renombrar un archivo
//Miselaneas
#define FAT_READ_SECTOR_COUNT     0x1    //Cantidad de sectores a leer del disco 
#define FAT_HREAD_ENTRY_DIR       0x2    //Se lee una entrada del directorio
#define FAT_HOPEN_DIR             0x1

struct SfatFormat32 {
    u8 bootJmp[3];
    u8 oemNamelabel[8];
    u16 bytesXSector;
    u8 sectorsXCluster;
    u16 reservedSectorCount;
    u8 numbFat;
    u16 rootEntryCount;
    u16 totalSectors16;
    u8 mediaType;
    u16 tableSize;
    u16 sectorsXTrack;
    u16 headSideCount;
    u32 hiddenSectorCount;
    u32 totalSectors32;
    //Cabecera para Fat 32        
    u32 tableSize32;
    u16 extendedFlags;
    u16 fatVersion;
    u32 rootCluster;
    u16 fatInfo;
    u16 backupBSSector;
    u8 reserved0[12];
    u8 driveNumber;
    u8 reserved1;
    u8 bootSignature;
    u32 volumeId;
    u8 volumeLabel[11];
    u8 fatTypeLabel[8];
} __attribute__((packed));

struct SfatDescriptor {
    u32 fatStartSectors; //Primer sector donde empieza la FAT no debe ser 0 Siemre 32    
    u32 fatSectors; //Cantidad de sectores ocupados por la fat   
    u32 fatSize; //Sectores ocupados por la tabla localizacion de Ficheros (FAT)
    u8 numberFat; //Numero de copia de la FAT
    u32 totalSector; //Total de sectores ocupado por toda la estructura del sistema fat
    u32 sectorXCluster; //Sector por cluster
    u32 bytesXSector; //byte por sectores
    u32 dataStartsSectors; //Primer sector para los datos se aloja despues de la FAT
    u32 dataSector; //Cantidad de sectores del contenido de los datos 
    u32 offsetSectorPartition; //Sector de comienzo de la particion FAT
} __attribute__((packed));

struct SdirFat32 {
    u8 name[11];
    u8 attrib;
    u8 nTres;
    u8 crtTimeTenth;
    u16 crtTime;
    u16 crtDate;
    u16 lstAccDate;
    u16 fstClusHI;
    u16 wrtTime;
    u16 wrtDate;
    u16 fstClusLO;
    u32 fileSize;
} __attribute__((packed));

struct SdirLFNFat32 {
    u8 ldirOrd;
    u8 ldirName1[10];
    u8 ldirAttrib;
    u8 ldirType;
    u8 ldirCheckSum;
    u8 ldirName2[12];
    u16 ldirFirstClusterL;
    u8 ldirName3[4];
} __attribute__((packed));

struct sfileHead {//almacena la ubicacion fisica del archivo y donde esta
    u32 phisicalEntryFat; //entrada en la FAT
    u32 sectorCount; //sectores desplazados 
    u32 index; //desplazamiento
} __attribute__((packed));

struct sfileBody {
    u32 entryFat; //entrada en la FAT
    u8 sectorCount; //sectores desplazados 
    u32 clusterChain; //cluster actual del archivo
    u32 ptrCurFile; //Puntero dentro del archivo  
    u32 phisicalEntryFat; //Entrada Fisica en la Fat
    u32 indexFile;
} __attribute__((packed));

struct SfileOpen {
    sfileHead fileHead; //posicion del archivo se usa para cambiar el tamño o atributos
    sfileBody fileBody; //estado del desplazamiento interno del archivo
    u32 fileSize; //Tamaño del archivo
} __attribute__((packed));

struct SdirOpen {
    u32 entryFat; //entrada en la tabla fat
    u32 phisicalEntryFat; //entrada fisica en el disco de comienzo de esta entrada en el area de datos de la fat
    u8 sectorCount; //sector actual en que esta la entrada
    u32 indexFile; //entrada en el directorio del proximo archivo o entrada
    u32 entryNextFile; //entrada del proximo archivo
    u32 clusterChain; //cluster actual del archivo
    u8 flags; //banderas
} __attribute__((packed));

#endif

