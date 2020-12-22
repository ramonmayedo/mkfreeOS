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
#include "iso9660.h"
#include "../architecture/x86/x86.h"
extern Sx86 x86;
extern Score core;

Ciso9660::Ciso9660(void *aiso9660Structure, int aidDisk, int aoffsetSectorPartition) {
    idDisk = aidDisk;
    core.string.strCopy(iso9660Descriptor.volumeStandarId, ((SprimaryVolumeDescriptor*) aiso9660Structure)->volumeStandarId, 5);
    core.string.strCopy(iso9660Descriptor.systemId, ((SprimaryVolumeDescriptor*) aiso9660Structure)->systemId, 32);
    core.string.strCopy(iso9660Descriptor.volumeId, ((SprimaryVolumeDescriptor*) aiso9660Structure)->volumeId, 32);

    core.string.strCopy(infoMedia.labelOem, iso9660Descriptor.volumeStandarId, sizeof (iso9660Descriptor.volumeStandarId));
    core.string.strCopy(infoMedia.labelType, iso9660Descriptor.systemId, 8); 
    core.string.strCopy(infoMedia.labelVolume, iso9660Descriptor.volumeId, 8);

    iso9660Descriptor.volumeSpaceSizeL = ((SprimaryVolumeDescriptor*) aiso9660Structure)->volumeSpaceSizeL;
    iso9660Descriptor.volumeSetSizeL = ((SprimaryVolumeDescriptor*) aiso9660Structure)->volumeSetSizeL;
    iso9660Descriptor.volumeSequencyNumbreL = ((SprimaryVolumeDescriptor*) aiso9660Structure)->volumeSequencyNumbreL;
    iso9660Descriptor.logicBlockSizeL = ((SprimaryVolumeDescriptor*) aiso9660Structure)->logicBlockSizeL;
    iso9660Descriptor.pathTableSizeL = ((SprimaryVolumeDescriptor*) aiso9660Structure)->pathTableSizeL;
    iso9660Descriptor.locationPathTableL = ((SprimaryVolumeDescriptor*) aiso9660Structure)->locationPathTableL;
    iso9660Descriptor.locationOptionalPathTableL = ((SprimaryVolumeDescriptor*) aiso9660Structure)->locationOptionalPathTableL;
    //
    iso9660Descriptor.rootDirectory.lengthDirectoryRecord = ((SprimaryVolumeDescriptor*) aiso9660Structure)->dirEntryForRootDir.lengthDirectoryRecord;
    iso9660Descriptor.rootDirectory.extendedAttributeRecordLength = ((SprimaryVolumeDescriptor*) aiso9660Structure)->dirEntryForRootDir.extendedAttributeRecordLength;
    iso9660Descriptor.rootDirectory.locationExtentLBAL = ((SprimaryVolumeDescriptor*) aiso9660Structure)->dirEntryForRootDir.locationExtentLBAL;
    iso9660Descriptor.rootDirectory.dataLengthL = ((SprimaryVolumeDescriptor*) aiso9660Structure)->dirEntryForRootDir.dataLengthB;
    core.string.strCopy(iso9660Descriptor.rootDirectory.recordDateTime, ((SprimaryVolumeDescriptor*) aiso9660Structure)->dirEntryForRootDir.recordDateTime, 7);
    iso9660Descriptor.rootDirectory.fileFlags = ((SprimaryVolumeDescriptor*) aiso9660Structure)->dirEntryForRootDir.fileFlags;
    iso9660Descriptor.rootDirectory.fileUnitSize = ((SprimaryVolumeDescriptor*) aiso9660Structure)->dirEntryForRootDir.fileUnitSize;
    iso9660Descriptor.rootDirectory.interleaveGapSizeFiles = ((SprimaryVolumeDescriptor*) aiso9660Structure)->dirEntryForRootDir.interleaveGapSizeFiles;
    iso9660Descriptor.rootDirectory.volumeSequenceNumberL = ((SprimaryVolumeDescriptor*) aiso9660Structure)->dirEntryForRootDir.volumeSequenceNumberL;
    iso9660Descriptor.rootDirectory.lengthFileIdentifier = ((SprimaryVolumeDescriptor*) aiso9660Structure)->dirEntryForRootDir.lengthFileIdentifier;

    iso9660Descriptor.pathTableSizeSector = iso9660Descriptor.pathTableSizeL / iso9660Descriptor.logicBlockSizeL + 1; //Cantidad de sectores logicos que tiene la Tabla;

    infoMedia.size =  iso9660Descriptor.volumeSpaceSizeL * iso9660Descriptor.logicBlockSizeL;
    
    gbuffer = new char[SIZE_BUFFER];
    gname = new char[255];
    fileInfo = new SfileInfo;
    delete((u8*) aiso9660Structure);
}

int Ciso9660::command(int acommand, char * astring) {
    scommand = acommand;
    u32 entry;
    switch (acommand) {
        case FILE_EXIST: return setDir(astring, &entry);
        case OPEN_DIR: return setDir(astring, &entry);
        case OPEN_FILE: return setDir(astring, &entry);
        case NEW_FOLDER: return COMMAND_INVALID;
        case NEW_FILE: return COMMAND_INVALID;
        case DELETE_FILE:return COMMAND_INVALID;
        case DELETE_FOLDER:return COMMAND_INVALID;
        default: return COMMAND_INVALID;
    }
}

int Ciso9660::commandFile(int acommand, char *astring, u32 asize, u32 aptr) {
    scommand = acommand;
    u32 size = asize*aptr;
    switch (acommand) {
        case READ_FILE: return readFile(astring, size); //fija la ruta a partir de la raiz
        case WRITE_FILE: return COMMAND_INVALID;
        case SEEK_CUR_FILE: return setPosFile(asize, aptr);
        case SEEK_SET_FILE: return setPosFile(asize, aptr);
        case SEEK_END_FILE: return setPosFile(asize, aptr);
        case GET_NEXT_FILE:
        {
            int error = getNextFile();
            if (error == 0) return 1;
            else return 0;
        }
        default: return COMMAND_INVALID;
    }
}

SfileInfo* Ciso9660::getFileInfo() {
    return fileInfo;
}

void Ciso9660::setDirPtr(void *aDirPtr) {
    dirCurrent = (SdirOpenIso9660*) aDirPtr;
}

void *Ciso9660::getDirPtr() {
    return (void*) dirCurrent;
}

void Ciso9660::setFilePtr(void *aFilePtr) {
    dirCurrent = (SdirOpenIso9660*) aFilePtr;
}

void *Ciso9660::getFilePtr() {
    return (void*) fileCurrent;
}

//Protected

int Ciso9660::setDir(char *adir, u32 *aEntryBlock) {
    int maxLenghtDirPath = core.string.strLenght(adir); //tamaño de la ruta

    if (maxLenghtDirPath == 0) {
        if (scommand == OPEN_FILE) return FILE_NO_EXIST;
        if (scommand == OPEN_DIR) return initDirOpen();
        return FILE_NO_EXIST;
    }

    int acceptHandle = 0;
    int error = 0, ptrIndex = 0;
    ScahePathSearch cahePathSearch;
    cahePathSearch.currentPathTLBA = 0; //LBA donde inicia la tabla
    cahePathSearch.currentBytePathT = 0; //Byte de incio en 0
    cahePathSearch.currentIndexEntryPath = 1; //Index del directorio padre incia en 1 que es el raiz
    cahePathSearch.currentIndexEntryPathSel = 1;
    cahePathSearch.locationExtentLBA = iso9660Descriptor.rootDirectory.locationExtentLBAL;
    readBlockDisk(iso9660Descriptor.locationPathTableL, gbuffer, 1); //Se carga el sector

    for (int i = 0; i <= maxLenghtDirPath; i++) {
        if (adir[i] != '/' && i != maxLenghtDirPath) {
            gname[ptrIndex] = adir[i];
            if (isValidChar(&gname[ptrIndex])) return NAME_INVALID;
            ptrIndex++;
            if (ptrIndex == 255) return NAME_MAX_LENGHT;
        } else {
            gname[ptrIndex] = 0x0;
            if (i == maxLenghtDirPath) acceptHandle = FIND_END;
            if (error = dirExist(gname, ptrIndex, &cahePathSearch, acceptHandle)) return error;
            ptrIndex = 0;
        }
    }
    if (scommand == OPEN_FILE) { //Si no es un archivo no se puede abrir
        if (fileInfo->typeFile != ATTRK_ARCHIVE) return FILE_NO_EXIST;
    } else if (scommand == OPEN_DIR) { //Si no es un directorio no se puede abrir
        if (fileInfo->typeFile != ATTRK_DIRECTORY) return FILE_NO_EXIST;
    }
    return 0;
}

int Ciso9660::dirExist(char *filename, u8 size, ScahePathSearch *cahePathSearch, int endFound) {
    //Busqueda por la tabla
    while (1) { //Se busca en todas las entradas de la tabla
        if (gbuffer[cahePathSearch->currentBytePathT] != 0) { //Si es 0 este byte entonces se busca el siguiente
            cahePathSearch->sizeName = gbuffer[cahePathSearch->currentBytePathT + 0]; //Tamaño del nombre
            u16 directoryNumberParentDirectory = *(u16*) ((u8*) gbuffer + cahePathSearch->currentBytePathT + 6);
            core.string.strCopy(fileInfo->name, (char*) (gbuffer + cahePathSearch->currentBytePathT + 8), cahePathSearch->sizeName);
            if (cahePathSearch->sizeName == size && cahePathSearch->currentIndexEntryPath == directoryNumberParentDirectory) {
                if (core.string.strCmp(fileInfo->name, filename, cahePathSearch->sizeName) == 0) {
                    cahePathSearch->locationExtentLBA = *(u32*) ((u8*) gbuffer + cahePathSearch->currentBytePathT + 2);
                    cahePathSearch->currentBytePathT = (cahePathSearch->currentBytePathT + 8 + cahePathSearch->sizeName);
                    cahePathSearch->currentIndexEntryPath = cahePathSearch->currentIndexEntryPathSel;

                    if (endFound) {
                        dirCurrent = new SdirOpenIso9660;
                        dirCurrent->currentIndexEntry = 0;
                        dirCurrent->currentLBA = cahePathSearch->locationExtentLBA;
                        dirCurrent->indexByteNextFile = 0;
                        dirCurrent->flags = ATTRK_DIRECTORY;
                        fileInfo->typeFile = ATTRK_DIRECTORY;
                    }
                    return 0;
                }
            }
            cahePathSearch->currentIndexEntryPathSel++;
            cahePathSearch->currentBytePathT += 8 + cahePathSearch->sizeName; //proxima entrada
        } else cahePathSearch->currentBytePathT++; //Por si es un byte en 0 despues de la entrada

        if (cahePathSearch->currentBytePathT >= iso9660Descriptor.logicBlockSizeL) {
            cahePathSearch->currentPathTLBA++;
            if (cahePathSearch->currentPathTLBA >= iso9660Descriptor.pathTableSizeSector) break; //Salgo a buscar en el directorio
            cahePathSearch->currentBytePathT = 0;
            readBlockDisk(iso9660Descriptor.locationPathTableL + cahePathSearch->currentPathTLBA, gbuffer, 1); //Se carga el sector
        }
    }
    return fileExist(filename, size, cahePathSearch); //Si no esta esta entrada en la tabla path se buca en la entradas al directorio como archivo
}

int Ciso9660::fileExist(char *filename, u8 size, ScahePathSearch *cahePathSearch) {
    readBlockDisk(cahePathSearch->locationExtentLBA, gbuffer, 1); //Se carga el sector
    cahePathSearch->currentByteDirRecord = 0;
    cahePathSearch->currenLocationExtLBA = 0;
    u32 sizeLogicDirRecord = cahePathSearch->currenLocationExtLBA;

    if (gbuffer[cahePathSearch->currentByteDirRecord + 33] == 0) { //Tamaño del dir record
        sizeLogicDirRecord += *(u32*) ((u8*) gbuffer + cahePathSearch->currentByteDirRecord + 10) / iso9660Descriptor.logicBlockSizeL;
    } else return ISO_ENTRY_ERROR_DIR0; //Error se espera deirectorio .

    while (1) { //Se busca en todas las entradas de la tabla
        if (gbuffer[cahePathSearch->currentByteDirRecord] != 0) { //Si es 0 este byte entonces se busca el siguiente
            u32 lengthDirectoryRecord = gbuffer[cahePathSearch->currentByteDirRecord];
            cahePathSearch->flags = *(u8*) ((u8*) gbuffer + cahePathSearch->currentByteDirRecord + 25); //Tamaño de la entrdad del directorio
            cahePathSearch->sizeName = *(u8*) ((u8*) gbuffer + cahePathSearch->currentByteDirRecord + 32);
            if (cahePathSearch->flags & 0x4) cahePathSearch->sizeName -= 2; //Tamaño del nombre
            if (gbuffer[cahePathSearch->currentByteDirRecord + 33] != 0 && gbuffer[cahePathSearch->currentByteDirRecord + 33] != 1) { //Si no el dir 1 . , y el 2 .. que tienen 0
                core.string.strCopy(fileInfo->name, (char*) ((u8*) gbuffer + cahePathSearch->currentByteDirRecord + 33), cahePathSearch->sizeName); //Secopia el nombre
                if (cahePathSearch->sizeName == size) { //Si tienen el mismo tamaño
                    if (core.string.strCmp(fileInfo->name, filename, cahePathSearch->sizeName) == 0) {
                        fileCurrent = new SfileOpenIso9660;
                        fileCurrent->locationFileLBA = *(u32*) ((u8*) gbuffer + cahePathSearch->currentByteDirRecord + 2);
                        fileCurrent->ptrCurFile = 0;
                        fileCurrent->fileSize = *(u32*) ((u8*) gbuffer + cahePathSearch->currentByteDirRecord + 10);
                        fileCurrent->currentByteLBA = 0;
                        fileCurrent->currentLBA = 0;
                        fileInfo->fileSize = fileCurrent->fileSize;
                        fileInfo->typeFile = ATTRK_ARCHIVE;
                        return 0;
                    }
                }
            }
            cahePathSearch->currentByteDirRecord += lengthDirectoryRecord;
        } else {

            return 2;
        };

        if (cahePathSearch->currentByteDirRecord >= iso9660Descriptor.logicBlockSizeL) {
            cahePathSearch->currenLocationExtLBA++;
            if (cahePathSearch->currentByteDirRecord >= sizeLogicDirRecord) return 1;
            cahePathSearch->currentByteDirRecord = 0;
            readBlockDisk(cahePathSearch->locationExtentLBA, gbuffer, 1); //Se carga el sector   siguiente
        }
    }

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

int Ciso9660::isValidChar(char *acar) {
    char format[20] = {'!', '\"', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', ' ', '<', '>', '?', '_'};
    if (*acar >= 'a' && *acar <= 'z') { //Se convierte a mayuscula
        *acar = *acar - 0x20;
        return 0;
    }
    if (*acar >= 'A' && *acar <= 'Z' || *acar >= '0' && *acar <= '9') return 0;
    for (int j = 0; j < 20; j++)
        if (*acar == format[j]) return 0;
    return 1;
}

void Ciso9660::openFilePtr(char *fileName, ScahePathSearch *cahePathSearch) {
    SfileOpenIso9660 *fileOpenIso9660 = new SfileOpenIso9660;
    fileOpenIso9660->currentLBA = cahePathSearch->locationFileLBA;
    fileOpenIso9660->ptrCurFile = 0;
    fileOpenIso9660->fileSize = cahePathSearch->fileSize;
}

void Ciso9660::openDirPtr(char *fileName, ScahePathSearch *cahePathSearch) {
    SdirOpenIso9660 *dirOpenIso9660 = new SdirOpenIso9660;
    dirOpenIso9660->currentIndexEntry = 0;
    dirOpenIso9660->currentLBA = cahePathSearch->locationExtentLBA;
    dirOpenIso9660->indexByteNextFile = 0;

}

int Ciso9660::getNextFile() {
    readBlockDisk(dirCurrent->currentLBA, gbuffer, 1); //Se carga el sector

    if (dirCurrent->indexNextFile == 0)
        if (gbuffer[dirCurrent->indexByteNextFile + 33] == 0) //Tamaño del dir record
            dirCurrent->dataLenght = dirCurrent->currentLBA + (*(u32*) ((u8*) gbuffer + 10)) / iso9660Descriptor.logicBlockSizeL;
        else return ISO_ENTRY_ERROR_DIR0;

    while (1) { //Se busca en todas las entradas de la tabla
        if (gbuffer[dirCurrent->indexByteNextFile] != 0) { //Si es 0 este byte entonces se busca el siguiente
            u32 lengthDirectoryRecord = gbuffer[dirCurrent->indexByteNextFile];
            dirCurrent->flags = *(u8*) ((u8*) gbuffer + dirCurrent->indexByteNextFile + 25); //Tamaño de la entrdad del directorio
            u8 sizeName = *(u8*) ((u8*) gbuffer + dirCurrent->indexByteNextFile + 32); //Siempre se agrega en los ficheros al final ;1
            if (dirCurrent->flags & 0x4 | dirCurrent->flags == 0) {
                if (dirCurrent->flags & 0x4) sizeName -= 2; //Tamaño del nombre'
                fileInfo->typeFile = ATTRK_ARCHIVE;
                fileInfo->fileSize = *(u32*) ((u8*) gbuffer + dirCurrent->indexByteNextFile + 10);
            }  else fileInfo->typeFile = ATTRK_DIRECTORY;
            if (gbuffer[dirCurrent->indexByteNextFile + 33] == 0) {
                fileInfo->name[0] = '.';
                fileInfo->name[1] = 0x0;
            } else if (gbuffer[dirCurrent->indexByteNextFile + 33] == 1) {
                fileInfo->name[0] = '.';
                fileInfo->name[1] = '.';
                fileInfo->name[2] = 0x0;
            } else
                core.string.strCopy(fileInfo->name, (char*) ((u8*) gbuffer + dirCurrent->indexByteNextFile + 33), sizeName); //Secopia el nombre
            dirCurrent->indexByteNextFile += lengthDirectoryRecord;
            dirCurrent->indexNextFile += 1;
            return 0;
        } else return 2;

        if (dirCurrent->indexByteNextFile >= iso9660Descriptor.logicBlockSizeL) {
            dirCurrent->currentLBA++;
            if (dirCurrent->currentLBA >= dirCurrent->dataLenght) return 1;
            dirCurrent->indexByteNextFile = 0;
            readBlockDisk(dirCurrent->currentLBA, gbuffer, 1);
        }
    }
}

int Ciso9660::readFile(char *abuffer, u32 asize) {
    if (fileCurrent->ptrCurFile >= fileCurrent->fileSize) return 0;
    u32 i = 0, size = asize, j = fileCurrent->currentByteLBA;
    if (fileCurrent->ptrCurFile + asize > fileCurrent->fileSize) size = fileCurrent->fileSize - fileCurrent->ptrCurFile;
    readBlockDisk(fileCurrent->locationFileLBA + fileCurrent->currentLBA, gbuffer, 1);

    while (i < size) {
        abuffer[i] = gbuffer[j];
        j++;
        i++;
        if (j == iso9660Descriptor.logicBlockSizeL) {
            j = 0;
            fileCurrent->currentLBA++;
            readBlockDisk(fileCurrent->locationFileLBA + fileCurrent->currentLBA, gbuffer, 1);
        }
    }
    fileCurrent->ptrCurFile += size;
    fileCurrent->currentLBA = j;
    return size;
}

u32 Ciso9660::setPosFile(u32 apos, u32 result) {
    u32 pos;

    if (scommand == SEEK_SET_FILE) { //Si es a partir del comienzo
        pos = (int) apos;
    } else if (scommand == SEEK_CUR_FILE) { //Cursor relativo
        pos = (int) fileCurrent->ptrCurFile + (int) apos; //Suma relativa
    } else if (scommand == SEEK_END_FILE) {
        pos = (int) fileCurrent->fileSize + (int) apos;
    }
    if (pos > fileCurrent->fileSize) pos = fileCurrent->fileSize; //Si es mayor va al final
    if (pos == fileCurrent->ptrCurFile) return 0;

    fileCurrent->currentLBA = pos / iso9660Descriptor.logicBlockSizeL;
    fileCurrent->ptrCurFile = pos;
    fileCurrent->currentByteLBA = pos % iso9660Descriptor.logicBlockSizeL;
    return 0;
}

int Ciso9660::readBlockDisk(u32 ablock, char *abuffer, int nBlock) {
    return core.cacheDisk.readSectorExt(idDisk, ablock, nBlock, abuffer);
}

int Ciso9660::writeBlockDisk(u32 ablock, char *abuffer, int nBlock) {

}

int Ciso9660::getKernelFlagsFile(int flagas) {
    if (flagas == ATTR_DIRECTORY_ISO9660) return ATTRK_DIRECTORY;
    if (flagas == ATTR_ARCHIVE_ISO9660) return ATTRK_ARCHIVE;
    return flagas;
}

int Ciso9660::initDirOpen() {
    dirCurrent = new SdirOpenIso9660;
    dirCurrent->currentLBA = iso9660Descriptor.rootDirectory.locationExtentLBAL;
    dirCurrent->flags = ATTRK_DIRECTORY;
    dirCurrent->currentIndexEntry = 0;
    dirCurrent->indexByteNextFile = 0;
    dirCurrent->indexNextFile = 1;
    return 0;
}

void Ciso9660::closeFile() {
    if (fileCurrent) delete(fileCurrent);
    if (dirCurrent) delete(dirCurrent);

    fileCurrent = 0;
    dirCurrent = 0;
}

void Ciso9660::getRootInfo() {
    core.string.strCopy(rootInfo.name, iso9660Descriptor.volumeId, sizeof (iso9660Descriptor.volumeId));
    rootInfo.handle = iso9660Descriptor.locationPathTableL;
    rootInfo.typeFile = ATTRK_VOLUME_ID;
}

SinfoMedia *Ciso9660::getInfoVolume() {
    return &infoMedia;
}