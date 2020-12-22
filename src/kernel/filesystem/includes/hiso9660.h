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

#ifndef HISO9660_H
#define HISO9660_H
#include "../../defines/types.h"
#include "../../core/filesystem.h"

#define ISO9660 0x91
#define ATTR_DIRECTORY_ISO9660 0x2
#define ATTR_ARCHIVE_ISO9660 0x4


struct SdirectoryRecord {
    u8 lengthDirectoryRecord;
    u8 extendedAttributeRecordLength;
    u32 locationExtentLBAL;
    u32 locationExtentLBAB;
    u32 dataLengthL;
    u32 dataLengthB;
    char recordDateTime[7];
    u8 fileFlags;
    u8 fileUnitSize;
    u8 interleaveGapSizeFiles;
    u16 volumeSequenceNumberL;
    u16 volumeSequenceNumberB;
    u8 lengthFileIdentifier;
} __attribute__((packed));

struct SprimaryVolumeDescriptor {
    u8 volumeDdescriptorType; //Volume Descriptor Type
    char volumeStandarId[5]; //Standard Identifier
    u8 volumeDecriptorVersion; //Volume Descriptor Version
    u8 unused0;
    char systemId[32]; //System Identifier
    char volumeId[32]; //Volume Identifier
    u8 unused1[8];
    u32 volumeSpaceSizeL; //Volume Space Size Little Endian
    u32 volumeSpaceSizeB; //Volume Space Size Big Endian
    u8 unused2[32];
    u16 volumeSetSizeL; //Volume Set Size Little Endian
    u16 volumeSetSizeB; //Volume Set Size Big Endian
    u16 volumeSequencyNumbreL; //Volume Sequence Number Little Endian
    u16 volumeSequencyNumbreB; //Volume Sequence Number Big Endian
    u16 logicBlockSizeL; //Logical Block Size Little Endian
    u16 logicBlockSizeB; //Logical Block Size Big Endian
    u32 pathTableSizeL; //Path Table Size Little Endian
    u32 pathTableSizeB; //Path Table Size Big Endian
    u32 locationPathTableL; //Location of Occurrence of Type L Path Table Little Endian
    u32 locationOptionalPathTableL; //Location of Optional Occurrence of Type L Path Table Little Endian
    u32 locationPathTableB; //Location of Occurrence of Type B Path Table Big Endian
    u32 locationOptionalPathTableB; //Location of Optional Occurrence of Type B Path Table Big Endian
    SdirectoryRecord dirEntryForRootDir; //Entrada al directorio raiz
    char volumeSetId[128]; //Volume Set Identifier
    char publisherId[128]; //Publisher Identifier
    char dataPrepareId[128]; //Data Preparer Identifier
    char applicationId[128]; //Application Identifier
    char copyrightFileId[38]; //Copyright File Identifier
    char abstractFileId[36]; //Abstract File Identifier
    char bibliogFileId[37]; //Bibliographic File Identifier
    char VolumeCreatDateTime[17]; //Volume Creation Date and Time
    char VolumeModDateTime[17]; //Volume Modification Date and Time
    char VolumeExpDateTime[17]; //Volume Expiration Date and Time
    char VolumeEffDateTime[17]; //Volume Effective Date and Time
    u8 fileStructureVersion; //File Structure Version
    u8 unused3;
    char applicationUsed[512]; //Application Used
    u8 unused4[653];
} __attribute__((packed));

struct SbootRecordVolumeDescriptor {
    u8 volumeDdescriptorType; //Volume Descriptor Type
    char volumeStandarId[5]; //Standard Identifier
    u8 volumeDecriptorVersion; //Volume Descriptor Version
    char systemId[32]; //System Identifier
    char bootId[32]; //Volume Identifier
    u32 bootCatalog; //As little-endian 32-bit number the block address of the El Torito Boot Catalo
    u8 unused[1973];
} __attribute__((packed));

struct SvolumeDescriptorTerminator {
    u8 volumeDdescriptorType; //Volume Descriptor Type
    char volumeStandarId[5]; //Standard Identifier
    u8 volumeDecriptorVersion; //Volume Descriptor Version
    u8 unused[2041];
} __attribute__((packed));


//El Torito Section Entry Extension

struct SvalidationEntry {
    u8 headerId; //Header ID
    u8 platformID; //Platform ID 0 = 80x86 1=Power PC 2=Mac
    u16 reserved; //Reserved, must be 0
    char idString[24]; //ID string. This is intended to identify the manufacturer/developer of the CD-ROM.
    u16 checksumWord; //Checksum Word. This sum of all the words in this record should be 0.
    u8 key1; //Key byte, must be 55. This value is included in the checksum.
    u8 key2; //Key byte, must be AA. This value is included in the checksum.
} __attribute__((packed));

struct SinitialDefaultEntry {
    u8 bootIndicator; //Boot Indicator. 88 = Bootable, 00 = Not Bootable
    u8 bootMediaType; //Boot media type 0 No Emulation
    u16 loadSegment; //Load Segment. This is the load segment for the initial boot image. If this value is 0 the system will use the traditional segment of 7C0
    u8 systemType; //System Type. This must be a copy of byte 5 (System Type) from the Partition Table found in the boot image.
    u8 unused1; //Unused, must be 0
    u16 sectorCount; //Sector Count. This is the number of virtual/emulated sectors the system will store at Load Segment during the initial boot procedure.
    u32 logicImageLBA; //Load RBA. This is the start address of the virtual disk. CD’s use Relative/Logical block addressing.
    u8 unused2[20]; //nused, must be 0.
} __attribute__((packed));


//Structuras para uso de la clase

struct SrootDirectoryRecordDescriptor {
    u8 lengthDirectoryRecord;
    u8 extendedAttributeRecordLength;
    u32 locationExtentLBAL;
    u32 dataLengthL;
    char recordDateTime[7];
    u8 fileFlags;
    u8 fileUnitSize;
    u8 interleaveGapSizeFiles;
    u16 volumeSequenceNumberL;
    u8 lengthFileIdentifier;
} __attribute__((packed));

struct SpathTableDescriptor {
    u8 lengthDirectoryIdentifier; //Length of Directory Identifier
    u8 extendedAttributeRecordLength; //Extended Attribute Record Length
    u32 locationExtentLB; //Location of Extent (LBA)
    u16 directoryNumberParentDirectory; //
    char ptrName; //
} __attribute__((packed));


#define SIZE_BUFFER 2048

struct SdirOpenIso9660 {
    u16 currentIndexEntry; //index en la tabla del directorio abierto
    u32 indexByteNextFile; //index a la siguiente entrada en la tabla, cuando este es el ultimo o no hay ninguno es 0
    u32 currentLBA; //entrada de la archivo en el disco
    u32 dataLenght;
    u16 indexNextFile; //index a la siguiente entrada en la tabla, cuando este es el ultimo o no hay ninguno es 0
    u8 flags; //banderas
} __attribute__((packed));

struct SfileOpenIso9660 {
    u32 locationFileLBA;
    u32 currentLBA; //Almacena el LBA actual en el disco
    u32 ptrCurFile; //Puntero atual dentro del fichero
    u32 fileSize; //Tamaño del fichero
    u32 currentByteLBA; //Nuemero de Byte dentro del LBA que se esta 
} __attribute__((packed));

struct ScahePathSearch {
    u32 currentPathTLBA; //Almacena en que sector logico se esta haciendo la busqueda tabla path
    u16 currentBytePathT; //Almacena en que Byte se quedo para continuar con la busqueda  tabla path
    u16 currentIndexEntryPath; //Almacena cual es su Index dentro de la Tabla
    u16 currentIndexEntryPathSel;
    u8 sizeName; //Almacena la cantidad de caracteres del nombre
    u32 locationExtentLBA; //Almacena en que sector logico esta el registro del directorio
    u32 currenLocationExtLBA; //Almacena en que sector va la busqueda en el registro del directorio
    u16 currentByteDirRecord; //Almacena en que byte se quedo para continuar con la busqueda en registro del directorio
    u32 locationFileLBA; //Almacena en que sector logico esta el contenido del archivo
    u32 fileSize; //Tamaño del archivo
    u8 flags; //Flag del archivo
} __attribute__((packed));

struct Siso9660Descriptor {
    char volumeStandarId[6];
    char systemId[33]; //System Identifier
    char volumeId[33]; //Volume Identifier
    u32 volumeSpaceSizeL; //Volume Space Size Little Endian
    u16 volumeSetSizeL; //Volume Set Size Little Endian
    u16 volumeSequencyNumbreL; //Volume Sequence Number Little Endian
    u16 logicBlockSizeL; //Logical Block Size Little Endian
    u32 pathTableSizeL; //Path Table Size Little Endian
    u32 locationPathTableL; //Location of Occurrence of Type L Path Table Little Endian
    u32 locationOptionalPathTableL; //Location of Optional Occurrence of Type L Path Table Little Endian
    SrootDirectoryRecordDescriptor rootDirectory; //
    u32 pathTableSizeSector; //
} __attribute__((packed));


#endif
