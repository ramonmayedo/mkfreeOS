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

#ifndef HBFS_H
#define HBFS_H


#include "../../../defines/types.h"
#include "../../filesystem.h"

#define BFS 0x90
#define RES_SECTOR_BFS  51




#define BLOCK_DATA_FILE      0x80
#define BLOCK_DATA_DIR       0x60
#define BLOCK_NODE           0x40

#define ATTR_DIRECTORY_BFS      0x60    //(Directory)
#define ATTR_ARCHIVE_BFS        0x40    //(Archive)

struct SblockHead {
    int prev;
    int next;
    int blockMap;
    int sizeMap;
} ;

struct SentryBlock{
    int type;
    int blockData;
};

struct SblockData {
    SentryBlock typeBlock;  //Tipo de Bloque  y Cluster Donde continuan los datos 
    int fileType;           //Tipo de Archivo
    int size;               //Tamño del archivo
    int indexEntry;         //Entrada en el directorio padre
    int parent;             //Dir del directorio padre
    int countCluster;       //Cantidad de Clusters
    char name[228];         //Nombre del Archivo
    SentryBlock entry[224];
};

struct SfbsPartition {
    char label[12];
    char version;
    int offset;
    int  byteXSector;
    char sectorXcluster;
    int sectorCount;
    int sectorStart;
    int sectorEnd;
    int sectorReserved;
    int countMap;
    char type[4];
    char resv[460];
};

struct SbfsDescriptor {
    int byteXSector;
    char sectorXcluster;
    int sectorCount;
    int sectorStart;
    int sectorEnd;
    int sectorReserved;
    int countMap;
    int offset;
} __attribute__((packed));

struct SblockNodeFile {
    SentryBlock typeBlock; //Tipo de Bloque  y Cluster Donde continuan los datos 
    int fileType; //Tipo de Archivo
    int size; //Tamño del archivo
    char res[8];
    char name[230]; //Nombre del Archivo
    char data[1792];
} __attribute__((packed));

struct SblockDataEntryDir { //Cluster para Dato de Directorio
    SentryBlock entry[256];
};

struct SblockDataEntryFile { //Cluster Para Dato de Archivos
    SentryBlock typeBlock;
    char data[2040];
};

struct SfileHeadBfs {         //almacena la ubicacion fisica del archivo y donde esta
    u32 entryBlock;           //entrada actual
    u32 phisicalEntryBlock;   //entrada en el bloque contiene nombre,tamaño...
} __attribute__((packed));

struct SfileBodyBfs {
    u32 entryBlock;          //entrada actual
    u32 entryNextBlock;      //entrada siguiente
    u32 ptrCurFile;          //Puntero dentro del archivo  
    u32 indexBlock;          //Index dentro del bloque que se lee
    u32 phisicalEntryBlock;  //Entrada Fisica
} __attribute__((packed));


struct SfileOpenBfs {
    SfileHeadBfs fileHead;     //posicion del archivo se usa para cambiar el tamño o atributos
    SfileBodyBfs fileBody;     //estado del desplazamiento interno del archivo
    u32 fileSize;           //Tamaño del archivo
} __attribute__((packed));

struct SdirOpenBfs {
    u32 entryBlock;          //entrada en la tabla
    u32 phisicalEntryBlock;  //entrada fisica en el disco de comienzo de esta entrada en el area de datos de la fat
    u32 indexFile;           //entrada en el directorio del proximo archivo o entrada
    u32 entryNextFile;       //entrada del proximo archivo
    u8 flags;                //banderas
} __attribute__((packed));

#endif

