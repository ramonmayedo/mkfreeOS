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

#include "fat.h"
#include "../architecture/x86/x86.h"

extern Sx86 x86;
extern Score core;
extern Smaps maps;

Cfat::Cfat(void *afatStrcture, int aidDisk, int aoffsetSectorPartition) {
    SfatFormat32 *fatFormat32 = (SfatFormat32*) afatStrcture;
    idDisk = aidDisk;
    core.string.strCopy(infoMedia.labelVolume, (char*) fatFormat32->volumeLabel, sizeof (fatFormat32->volumeLabel));
    core.string.strCopy(infoMedia.labelType, (char*) fatFormat32->fatTypeLabel, sizeof (fatFormat32->fatTypeLabel));
    core.string.strCopy(infoMedia.labelOem, (char*) fatFormat32->oemNamelabel, sizeof (fatFormat32->oemNamelabel));

    fatDescriptor.fatSize = fatFormat32->tableSize32;
    fatDescriptor.fatStartSectors = fatFormat32->reservedSectorCount;
    fatDescriptor.numberFat = fatFormat32->numbFat;
    fatDescriptor.fatSectors = fatDescriptor.numberFat * fatDescriptor.fatSize;
    fatDescriptor.totalSector = fatFormat32->totalSectors32;
    fatDescriptor.bytesXSector = fatFormat32->bytesXSector;
    fatDescriptor.dataStartsSectors = fatDescriptor.fatStartSectors + fatDescriptor.fatSectors;
    fatDescriptor.sectorXCluster = fatFormat32->sectorsXCluster;
    fatDescriptor.offsetSectorPartition = aoffsetSectorPartition;

    infoMedia.size = (fatDescriptor.totalSector + fatDescriptor.offsetSectorPartition) * fatDescriptor.bytesXSector;
    fatSectorStart = fatDescriptor.fatStartSectors + fatDescriptor.offsetSectorPartition;
    dataSectorStart = fatDescriptor.dataStartsSectors + fatDescriptor.offsetSectorPartition;
    sizeBuff = fatDescriptor.bytesXSector*FAT_READ_SECTOR_COUNT;
    maxSizeEntryDir = sizeBuff / 32;
    maxSizeEntryFat = sizeBuff / 4;
    gname = new char[256];
    ename = new char[256];
    shortName = new char[11];
    gbuffer = new char[sizeBuff];
    ebuffer = new char[sizeBuff];
    getRootInfo(); //informacion del volumen 
    delete((u8*) afatStrcture);
    fileInfo =0;
    phisicalEntryFatWorkDir = dataSectorStart;
}

int Cfat::command(int acommand, char * astring) {
    scommand = acommand;
    u32 entry=phisicalEntryFatWorkDir;
    switch (acommand) {
        case SET_DIR_WORK: return setDir(astring,&entry); //fija la ruta a partir de la raiz
        case OPEN_DIR:    return setDir(astring,&entry);
        case OPEN_FILE:   return setDir(astring,&entry);
        case NEW_FOLDER: return newFile(astring, ATTR_DIRECTORY);
        case NEW_FILE: return newFile(astring, ATTR_ARCHIVE);
        default: return COMMAND_INVALID;
    }
}

int Cfat::commandFile(int acommand, char *astring, u32 asize, u32 aptr) {
    scommand = acommand;
    switch (acommand) {
        case READ_FILE: return readFile(astring, asize*aptr); //fija la ruta a partir de la raiz
        case WRITE_FILE: return writeFile(astring, asize*aptr);
        case SEEK_CUR_FILE: return setPosFile(asize);
        case SEEK_SET_FILE: return setPosFile(asize);
        case SEEK_END_FILE: return setPosFile(asize);
        case GET_NEXT_FILE:
        {
            int error = fileExistDir(&dirCurrent->phisicalEntryFat, astring, 1);
            if (error == 0) return 1;
            else return 0;
        }
        default: return COMMAND_INVALID;
    }
}

void *Cfat::getFilePtr() {
    return (void*) fileCurrent;
}

void *Cfat::getDirPtr() {
    return (void*) dirCurrent;
}

void Cfat::setFilePtr(void *aFilePtr) {
    fileCurrent = (SfileOpen*) aFilePtr;
}

void Cfat::setDirPtr(void *aDirPtr) {
    dirCurrent = (SdirOpen*) aDirPtr;
}

SinfoMedia *Cfat::getInfoVolume() {
    return &infoMedia;
}

SfileInfo* Cfat::getFileInfo() {
    return fileInfo;
}

//Funciones Protected solo se usan dentro de la propia clase de forma auxiliar

int Cfat::setDir(char *adir,u32 *aEntryFat) {
    int maxLenghtDirPath = core.string.strLenght(adir); //tamaño de la ruta
    if (maxLenghtDirPath == 0) {
        if (scommand == OPEN_FILE ) return FILE_NO_EXIST;
        initDirOpen();
        return 0;
    }
    int error = 0, ptrIndex = 0, acceptHandle = 0;
    for (int i = 0; i <= maxLenghtDirPath; i++) {
        if (adir[i] != '/' && i != maxLenghtDirPath) {
            gname[ptrIndex] = adir[i];
            ptrIndex++;
            if (ptrIndex == 255) return NAME_MAX_LENGHT;
        } else {
            gname[ptrIndex] = 0x0;
            if (error = isValidName(gname)) return error;
            if (i == maxLenghtDirPath) acceptHandle = FAT_HOPEN_DIR;
            if (error = fileExistDir(aEntryFat, gname, acceptHandle)) return error;
            ptrIndex = 0;
        }
    }

    if (scommand == OPEN_FILE) {
        if (fileInfo->typeFile != ATTRK_ARCHIVE) return FILE_NO_EXIST;
    } else if (scommand == OPEN_DIR) { //Si no es un directorio no se puede abrir
        if (fileInfo->typeFile != ATTRK_DIRECTORY) return FILE_NO_EXIST;
    }
    //if (scommand == SET_DIR_WORK) phisicalEntryFatWorkDir = entryDir;
    return 0;
}

int Cfat::fileExistDir(u32 *aEntryFat, char *aname, int acceptHandle) {
    int readSector, file = 0, indexDir, partDirName, sizeLFN;
    u32 nextClusterChain, entryFat = *aEntryFat;
    if (scommand == GET_NEXT_FILE) {
        readSector = dirCurrent->sectorCount;
        indexDir = dirCurrent->indexFile;
        nextClusterChain = dirCurrent->clusterChain;
        entryFat = dirCurrent->phisicalEntryFat;
    } else {
        readSector = 0;
        indexDir = 0;
        nextClusterChain = entryFat; //hay que revisar
    }
    for (int i = 0; i < 255; i++) ename[i] = 0x0;
    
    if (indexDir < maxSizeEntryDir) readBlockDisk(entryFat + readSector, gbuffer);
    
    SdirFat32 *dirFat = (SdirFat32*) (gbuffer);
    SdirLFNFat32* dirLFNFat = (SdirLFNFat32*) (gbuffer);   
    
    if (indexDir == 0) if (dirFat[0].attrib == ATTR_VOLUME_ID) indexDir = 1;
    
    while (1) {
        if (indexDir == maxSizeEntryDir) {
            readSector += FAT_READ_SECTOR_COUNT;
            if (readSector == fatDescriptor.sectorXCluster) {//si el proximo sector esta en otro cluster        
                entryFat = nextClusterChainDir(&nextClusterChain);
                if (entryFat == 0) return FILE_NO_EXIST;
                readSector = 0;
            }
            readBlockDisk(entryFat + readSector, gbuffer);
            indexDir = 0;
        }

        if (dirFat[indexDir].name[0] == 0x0) return FILE_NO_EXIST;
        if (dirFat[indexDir].name[0] != 0xE5) {
            if (dirFat[indexDir].attrib == ATTR_DIRECTORY || dirFat[indexDir].attrib == ATTR_ARCHIVE) {
                core.string.strCopy(ename, (char*) dirFat[indexDir].name, sizeof (dirFat[indexDir].name));
            } else if (dirFat[indexDir].attrib == ATTR_LONG_FILE_NAME) {
                sizeLFN = dirFat[indexDir].name[0] - 0x40;
                if ((sizeLFN < 1 || sizeLFN > 20))return FAT_LFN_ENTRY_INVALID;
                if (indexDir + sizeLFN >= maxSizeEntryDir) {
                    partDirName = maxSizeEntryDir - indexDir;
                    extractFileNameLFN(indexDir, partDirName, sizeLFN, dirLFNFat, ename); //se extrae la primera parte del nombre
                    readSector += FAT_READ_SECTOR_COUNT;
                    if (readSector == fatDescriptor.sectorXCluster) {//si el proximo sector esta en otro cluster        
                        entryFat = nextClusterChainDir(&nextClusterChain);
                        if (entryFat == 0) return FAT_LFN_ENTRY_INVALID;
                        readSector = 0;
                    }
                    readBlockDisk(entryFat + readSector, gbuffer);
                    indexDir = (sizeLFN - partDirName); //cantidad que estan en el otro sector
                    extractFileNameLFN(0, indexDir, 0, dirLFNFat, ename); //se extrae parte del nombre que falta  
                } else {
                    extractFileNameLFN(indexDir, sizeLFN, 0, dirLFNFat, ename); //se extrae el nombre que esta completo en el mismo sector
                    indexDir += sizeLFN;
                }
            }//else return FAT_ENTRY_DIR_CORRUPT;

            if (scommand == GET_NEXT_FILE) {
                dirCurrent->indexFile = indexDir + 1; //apunta a la proxima entrada saltandose al nombre corto
                file = setFileInfo(&dirFat[indexDir], ename, acceptHandle);
                dirCurrent->entryNextFile = file;
                dirCurrent->clusterChain = nextClusterChain;
                dirCurrent->phisicalEntryFat = entryFat;
                dirCurrent->sectorCount = readSector;
                return 0;
            }

            if (core.string.strCmp(aname, ename, core.string.strLenght(aname)) == 0) { //si es el directorio                     
                file = setFileInfo(&dirFat[indexDir], aname, acceptHandle); //actualizo el manejador
                if (scommand == OPEN_FILE && acceptHandle == FAT_HOPEN_DIR) {
                    fileCurrent->fileHead.index = indexDir;
                    fileCurrent->fileHead.phisicalEntryFat = entryFat;
                    fileCurrent->fileHead.sectorCount = readSector;
                }
                *aEntryFat = file;
                return 0;
            }
        }
        indexDir++;

    }
}

int Cfat::newFile(char *anameFile, int atypeFile) {
    int error;
    u32 entryDir = phisicalEntryFatWorkDir, beforeEntryDir = 0; //entrada al directorio actual
    if (setDir(anameFile,&entryDir) == 0) return FILE_EXIST;
    if ((error = isValidName(gname))) return error; //se chekea que sea un nombre valido
    int partName = core.string.strLenght(gname) / 13 + 1; //partes del nombre/13 + el nombre corto(1)
    int indexEntry = 0;
    error = getEntryDirectoryFree(gbuffer, partName, &beforeEntryDir, &indexEntry, &entryDir);
    if (error) return error;
    convertNameToShort(gname, shortName); //se obtiene el nombre corto
    SdirLFNFat32* dirLFNFat = (SdirLFNFat32*) (gbuffer);
    SdirFat32 *dirFat = (SdirFat32*) (gbuffer);
    //--------------------------------------------------------------------------------- 
    u32 entryFat = 0;
    if (atypeFile == ATTR_DIRECTORY) {
        entryFat = getEntryFatFree(END_CHAIN_F); //se busca espacio libre en la FAT
        if (entryFat == 0) return FAT_FAT_ENTRY_FULL;
    }
    //Se alamacena el nombre en las entradas del directorio 
    u8 checkSum = checkSumCreate(shortName); //suma de chekeo del nombre
    for (int indexDir = 0; indexDir <= partName; indexDir++) {
        int index = indexEntry - indexDir;
        if (indexDir == 0) { //se almacena el nombre corto
            core.string.strCopy((char*) dirFat[index].name, shortName, sizeof (dirFat[index].name));
            setAttribFile(&dirFat[index], entryFat, atypeFile);
        } else { //se almacena el nombre largo
            convertToNameLarge(gname, &dirLFNFat[index], indexDir - 1);
            int ldirOrd;
            if (indexDir == partName) ldirOrd = 0x40 | partName;
            else ldirOrd = indexDir;
            setAttribFileLFN(&dirLFNFat[index], ldirOrd, checkSum);
        }
        if (index == 0 && indexDir < partName) { //si la entrada contiene parte en el sector anterior                  
            writeBlockDisk(entryDir, gbuffer); //se salva los datos de este sector que se agregaron
            if (beforeEntryDir != 0) entryDir = beforeEntryDir; //si tiene parte en el cluster anterior
            entryDir -= FAT_READ_SECTOR_COUNT;
            indexEntry = maxSizeEntryDir + indexDir; //para leer del final de sector hacia atras lo que falta del nombre   
            readBlockDisk(entryDir, gbuffer); //se carga datos del sector anterior para almacenar la parte que queda
        }
    }
    writeBlockDisk(entryDir, gbuffer); //se almacena el ultimo sector modificado 
    if (atypeFile == ATTR_DIRECTORY) createSubDir(entryFat, dirCurrent->entryFat); //se crean las entradas de los subdirectorios
    return 0;
}

int Cfat::extractFileNameLFN(int aindexDir, int asize, int aoffset, SdirLFNFat32* adirLFNFat, char *aname) {
    int index = 0, offset;
    if (aoffset == 0) offset = asize;
    else offset = aoffset;
    for (int j = 0; j < asize; j++) {
        int k = 0;
        index = (offset - j - 1)*13;
        while (k < 31) {
            aname[index] = adirLFNFat[aindexDir + j].ldirName1[k];
            index++;
            if (k == 8) k = 11;
            if (k == 23) k = 25;
            k += 2;
        }
    }
    return 0;
}

int Cfat::getAttrib(int aattrib) {
    if (aattrib & ATTR_DIRECTORY)
        return ATTRK_DIRECTORY;
    if (aattrib & ATTR_ARCHIVE)
        return ATTRK_ARCHIVE;
    if (aattrib & ATTR_VOLUME_ID)
        return ATTRK_VOLUME_ID;
    if (aattrib == 0)
        return ATTRK_ARCHIVE;
    return aattrib;
}

int Cfat::convertNameToShort(char *anameLarge, char *anameShort) {
    int sizeName = core.string.strLenght(anameLarge);
    int maxSize;
    if (sizeName > 8) {
        maxSize = 6;
        anameShort[6] = '~';
        anameShort[7] = '0';
        anameShort[8] = ' ';
        anameShort[9] = ' ';
        anameShort[10] = ' ';
    } else {
        maxSize = sizeName;
        for (int i = maxSize; i < 11; i++) anameShort[i] = ' ';
    }
    for (int i = 0; i < maxSize; i++) {
        if (anameLarge[i] >= 'a' && anameLarge[i] <= 'z')
            anameShort[i] = anameLarge[i] - 0x20;
        else if (anameLarge[i] == ' ') anameShort[i] = '_';
        else anameShort[i] = anameLarge[i];
    }
    return 0;
}

u32 Cfat::getEntryFatFree(u32 anewEntryValue) {
    u32 entryDir = fatSectorStart;
    u32 *fatTable = (u32*) (ebuffer);
    int readSector = 0, indexFat = 0;
    readBlockDisk(entryDir, ebuffer); //se lee primer sector de la fat
    while (1) {
        if (fatTable[indexFat] == 0x0) {
            fatTable[indexFat] = anewEntryValue;
            writeBlockDisk(entryDir + readSector, ebuffer); //se escribe la nueva entrada ocupada 
            return indexFat + maxSizeEntryFat*readSector;
        }
        indexFat++;
        if (indexFat == maxSizeEntryFat) {//si se llega al tamaño maximo de sectores de la fat
            readSector += FAT_READ_SECTOR_COUNT;
            if (readSector == fatDescriptor.fatSize) return 0;
            readBlockDisk(entryDir + readSector, ebuffer); //se lee proximo sector de la fat
            indexFat = 0;
        }
    }
    return 0;
}

int Cfat::convertToNameLarge(char *anameLarge, SdirLFNFat32 *adirLFNFat32, int aoffset) {
    int sizeName = core.string.strLenght(anameLarge); //tamaño del nombre
    int k = 0;
    char car;
    for (int i = 0; i < 13; i++) { //tamaño maximo de la parte del nombre LFN es 13
        if (i > sizeName) car = ' ';
        else car = anameLarge[i + aoffset * 13]; //offset se usa cuando el nombre esta en diferentes sectores
        adirLFNFat32->ldirName1[k] = car; //parte 1 del nombre k=0 a k=9 5 caracteres 
        k++;
        adirLFNFat32->ldirName1[k] = 0x0;
        if (k == 9) k = 12; //parte 2 del nombre k=12 a k=24 6 caracteres
        if (k == 24) k = 26; //parte 3 del nombre k=26 a k=30 2 caracteres
        k++;
    }
    return 0;
}

u8 Cfat::checkSumCreate(char *aname) {
    u8 sum = 0;
    for (int i = 0; i < 11; i++)
        sum = (sum >> 1) + (sum << 7) + aname[i];
    return sum;
}

u16 Cfat::getTime() {
    u16 time;
    core.clock.refreshTime();
    time = maps.clockMap.second | maps.clockMap.minute << 5 | maps.clockMap.hour << 11;
    return time;
}

u16 Cfat::getDate() {
    u16 date;
    date = maps.clockMap.day | maps.clockMap.month << 5 | (maps.clockMap.year + 20) << 9;
    return date;
}

int Cfat::isValidChar(char acar) {
    char format[18] = {'!', '#', '$', '%', '&', '\'', '(', ')', '-', '@', '^', '_', '`', '{', '}', '~', ' ', '.'};
    for (int j = 0; j < 18; j++)
        if (acar == format[j]) return 1;
    return 0;
}

int Cfat::isValidName(char *aname) {
    int sizeName = core.string.strLenght(aname); //tamaño del nomber
    if (sizeName > 255) return NAME_MAX_LENGHT; //si tiene mas 255 error
    char car;
    for (int i = 0; i < sizeName; i++) {
        car = aname[i];
        if ((car >= 'a' && car <= 'z' || car >= 'A' && car <= 'Z' || car >= '0' && car <= '9' || isValidChar(car))) {
        } else return NAME_INVALID;
    }
    return 0;
}

int Cfat::createSubDir(u32 aentryFat, u32 aparentEntry) {
    u32 entryFat = clearCluster(aentryFat, ebuffer); //se borra el cluster asignado para el directorio 
    SdirFat32 *dirFat = (SdirFat32*) (ebuffer);
    dirFat[0].name[0] = '.';
    dirFat[1].name[0] = '.';
    dirFat[1].name[1] = '.';
    for (int i = 1; i < 8; i++) {
        if (i > 1) dirFat[1].name[i] = ' ';
        dirFat[0].name[i] = ' ';
    }
    setAttribFile(&dirFat[0], aentryFat, ATTR_DIRECTORY);
    setAttribFile(&dirFat[1], aparentEntry, ATTR_DIRECTORY);
    writeBlockDisk(entryFat, ebuffer); //se almacenan los directorios . y ..
    return 0;
}

void Cfat::setAttribFile(SdirFat32 *adirFat, u32 aentryFat, int afileType) {
    adirFat->fileSize = 0;
    adirFat->attrib = afileType;
    adirFat->fstClusLO = aentryFat;
    adirFat->fstClusHI = aentryFat >> 16;
    adirFat->wrtDate = getDate();
    adirFat->wrtTime = getTime();
    adirFat->crtDate = adirFat->wrtDate;
    adirFat->crtTime = adirFat->wrtTime;
    adirFat->crtTimeTenth = 1;
    adirFat->lstAccDate = 1;
}

void Cfat::setAttribFileLFN(SdirLFNFat32 *adirLFNFat, int aldirOrd, u8 acheckSum) {
    adirLFNFat->ldirOrd = aldirOrd;
    adirLFNFat->ldirFirstClusterL = 0x0;
    adirLFNFat->ldirType = 0x0;
    adirLFNFat->ldirAttrib = ATTR_LONG_FILE_NAME;
    adirLFNFat->ldirCheckSum = acheckSum;
}

void Cfat::getRootInfo() {
    readBlockDisk(dataSectorStart, ebuffer);
    SdirFat32 *dirFat = (SdirFat32*) ebuffer;
    if (dirFat[0].attrib == ATTR_VOLUME_ID)
        core.string.strCopy(rootInfo.name, (char*) dirFat[0].name, sizeof (dirFat[0].name));
    rootInfo.handle = dataSectorStart;
    rootInfo.typeFile = ATTR_VOLUME_ID;
    initDirOpen();
}

int Cfat::setFileInfo(SdirFat32 *adirFat, char *aname, int aacceptHandle) {
    u32 file1 = adirFat->fstClusLO | adirFat->fstClusHI << 16; //entrada actual en la fat
    if (file1 == 0) file1 = 2; // 
    u32 file = getPhisicalBlockData(file1); //posicion fisica en el disco
    if (aacceptHandle) { //actualiza el estado actual 
        if (fileInfo == 0)  fileInfo = new SfileInfo;
        core.string.strCopy(fileInfo->name, aname, core.string.strLenght(aname));
        fileInfo->handle = file;
        fileInfo->typeFile = getAttrib(adirFat->attrib);
        fileInfo->fileSize = adirFat->fileSize;
        if (scommand == OPEN_FILE) {
            fileCurrent = new SfileOpen; //Se crea el archivo
            fileCurrent->fileBody.entryFat = file1;
            fileCurrent->fileBody.phisicalEntryFat = file;
            fileCurrent->fileBody.clusterChain = file1;
            fileCurrent->fileBody.ptrCurFile = 0;
            fileCurrent->fileBody.sectorCount = 0;
            fileCurrent->fileSize = adirFat->fileSize;
            fileCurrent->fileBody.indexFile = 0;
        } else if (scommand == OPEN_DIR || scommand == SET_DIR_WORK) {
            dirCurrent = new SdirOpen;
            dirCurrent->entryFat = file1;
            dirCurrent->phisicalEntryFat = file;
            dirCurrent->indexFile = 0;
            dirCurrent->entryNextFile = 0;
            dirCurrent->sectorCount = 0;
            dirCurrent->clusterChain = file1;
        }
    }
    return file;
}

int Cfat::getEntryDirectoryFree(char *buffer, int apartName, u32 *abeforeEntryDir, int *aindexEntry, u32 *aentryDir) {
    readBlockDisk(*aentryDir, buffer);
    SdirFat32 *dirFat = (SdirFat32*) (buffer);
    u32 entryBlock = dirCurrent->entryFat;
    int freeEntry = 0, readSector = 0, indexDir = 0;
    while (1) { //recorro las entradas del directorio
        if (dirFat[indexDir].name[0] == 0x0 || dirFat[indexDir].name[0] == 0xE5) freeEntry++; //si hay entradas vacias las cuento
        else {
            freeEntry = 0;
            *abeforeEntryDir = 0;
        } //si ya esta ocupado reinicio los contadores
        if (freeEntry > apartName) {
            *aindexEntry = indexDir;
            return 0;
        } //si se encuentran suficientes entradas para el fichero se finaliza
        indexDir++;
        if (indexDir == maxSizeEntryDir) { //Se recorrio un sector completo
            indexDir = 0;
            readSector += FAT_READ_SECTOR_COUNT;
            (*aentryDir) += FAT_READ_SECTOR_COUNT;
            if (readSector == fatDescriptor.sectorXCluster) { //se recorrio un cluster completo
                *abeforeEntryDir = *aentryDir; //entradas en el cluster anterior
                *aentryDir = setClusterChain(&entryBlock); //proximo cluster para buscar
                if (*aentryDir == 0) return FAT_FAT_ENTRY_FULL; //la fat esta llena
                readSector = 0;
            }
            readBlockDisk(*aentryDir, buffer); //se lee un sector para seguir buscando entrdas
        }
    }
    return 0;
}

u32 Cfat::setClusterChain(u32 *aentryFat) {
    int nextClusterChain = getClusterChain(*aentryFat); //proximo cluster chain del fichero
    if (nextClusterChain == 0x0) { //no tiene mas cluster chain por lo que hay que crearlo 
        u32 indexFat;
        u32 entryFat = getPhisicalEntryFat(&indexFat, *aentryFat); //Entrada del cluster a ampliar  
        u32 *fatTable = (u32*) (ebuffer);
        nextClusterChain = getEntryFatFree(END_CHAIN_F); //Nueva entrada de la fat
        if (nextClusterChain == 0) return 0;
        readBlockDisk(entryFat, ebuffer);
        fatTable[indexFat] = nextClusterChain; //cambio el cluster como cadena y le asigno el proximo
        writeBlockDisk(entryFat, ebuffer);
    }
    *aentryFat = nextClusterChain;
    return getPhisicalBlockData(nextClusterChain);
}

u32 Cfat::clearCluster(u32 aentryFat, char *abuffer) {
    u32 entryFat = getPhisicalBlockData(aentryFat); //se obtiene el comienzo del bloque de dato 
    for (int i = 0; i < fatDescriptor.bytesXSector; i++) //se incializan los datos a 0x0
        abuffer[i] = 0x0;
    for (int i = 0; i < fatDescriptor.sectorXCluster; i++) //se escriben las cantidad de sectores del cluster
        writeBlockDisk((entryFat + i), abuffer);
    return entryFat;
}

u32 Cfat::getClusterChain(u32 aentryFat) {
    u32 indexFat;
    u32 entryFat = getPhisicalEntryFat(&indexFat, aentryFat); //entrada fisica de la fat en el disco
    readBlockDisk(entryFat, ebuffer); //Se lee un bloque del disco
    u32 *fatTable = (u32*) (ebuffer);
    int nexClusterChain = 0;
    if (fatTable[indexFat] >= USED_S && fatTable[indexFat] <= USED_F) //Si este archivo tiene un clsuter chain(cadena)
        nexClusterChain = fatTable[indexFat]; //siguiente cluster      
    return nexClusterChain;
}

u32 Cfat::nextClusterChainDir(u32 *anextClusterChain) {
    u32 nextClusterChain = getClusterChain(*anextClusterChain);
    if (nextClusterChain == 0) return 0;
    *anextClusterChain = nextClusterChain;
    return getPhisicalBlockData(nextClusterChain);
}

int Cfat::getPhisicalBlockData(u32 ablockEntry) {
    return (ablockEntry - 2)*fatDescriptor.sectorXCluster + dataSectorStart;
}

u32 Cfat::getPhisicalEntryFat(u32 *aindex, u32 aentryFat) {
    *aindex = (aentryFat) % (fatDescriptor.bytesXSector / 4);
    u32 entryFat = fatDescriptor.fatStartSectors + fatDescriptor.offsetSectorPartition;
    entryFat += (aentryFat) / (fatDescriptor.bytesXSector / 4);
    return entryFat;
}

int Cfat::readBlockDisk(u32 ablock, char *abuffer) {
    return core.cacheDisk.readSector(idDisk, ablock, FAT_READ_SECTOR_COUNT, abuffer);
    //return core.diskidePIO->readSector(idDisk, ablock, READ_SECTOR_COUNT, abuffer);
}

int Cfat::writeBlockDisk(u32 ablock, char *abuffer) {
    return core.cacheDisk.writeSector(idDisk, ablock, FAT_READ_SECTOR_COUNT, abuffer);
    // return core.diskidePIO->writeSector(idDisk, ablock, READ_SECTOR_COUNT, abuffer);
}

void Cfat::initDirOpen() {
    dirCurrent = new SdirOpen;
    dirCurrent->entryFat = 2;
    dirCurrent->phisicalEntryFat = dataSectorStart;
    dirCurrent->indexFile = 0;
    dirCurrent->entryNextFile = 0;
    dirCurrent->sectorCount = 0;
    dirCurrent->clusterChain = 2;
    phisicalEntryFatWorkDir = dataSectorStart;
    //fileInfo = &rootInfo;
}
//----------Manejo de Archivo---------------------------------------------------

int Cfat::readFile(char *abuffer, u32 asize) {
    if (fileCurrent->fileBody.ptrCurFile >= fileCurrent->fileSize) return 0;
    u32 entryFat = fileCurrent->fileBody.phisicalEntryFat;
    u32 readSector = fileCurrent->fileBody.sectorCount;
    u32 nextClusterChain = fileCurrent->fileBody.clusterChain;
    u32 j = fileCurrent->fileBody.indexFile, i = 0;
    
    if (j != 0) readBlockDisk(entryFat + readSector, gbuffer);
    while (i < asize) {
        if (j == 0) {
            if (readSector == fatDescriptor.sectorXCluster) {
                entryFat = nextClusterChainDir(&nextClusterChain);
                readSector = 0;
            }
            readBlockDisk(entryFat + readSector, gbuffer);
        }
        abuffer[i] = gbuffer[j];
        j++;
        i++;
        if (j == sizeBuff) {
            j = 0;
            readSector += FAT_READ_SECTOR_COUNT;
        }
    }
    fileCurrent->fileBody.clusterChain = nextClusterChain;
    fileCurrent->fileBody.phisicalEntryFat = entryFat;
    fileCurrent->fileBody.sectorCount = readSector;
    fileCurrent->fileBody.indexFile = j;
    if (fileCurrent->fileBody.ptrCurFile + asize > fileCurrent->fileSize)
        asize = fileCurrent->fileSize - fileCurrent->fileBody.ptrCurFile;
   fileCurrent->fileBody.ptrCurFile += asize;
    return asize;
}

int Cfat::writeFile(char *abuffer, u32 asize) {
  //   x86.ioScreen.printf("a");
    if (fileCurrent->fileHead.phisicalEntryFat == 0) return 0;
    if ((fileCurrent->fileBody.ptrCurFile + asize) > fileCurrent->fileSize) setCurrentFileSize(asize);
    u32 readSector = fileCurrent->fileBody.sectorCount;
    u32 nextClusterChain = fileCurrent->fileBody.clusterChain, entryFat = fileCurrent->fileBody.phisicalEntryFat;
    u32 j = fileCurrent->fileBody.indexFile, i = 0;
//x86.ioScreen.printf("b");
    if (j != 0) readBlockDisk(entryFat + readSector, gbuffer);
    while (i < asize) {
        if (j == 0) {
            if (readSector == fatDescriptor.sectorXCluster) {
                entryFat = nextClusterChainDir(&nextClusterChain);
                readSector = 0;
                if (entryFat == 0) entryFat = setClusterChain(&nextClusterChain);
            }
        }
        gbuffer[j] = abuffer[i];
        j++;
        i++;
        if (j == sizeBuff) {
            j = 0;
            writeBlockDisk(entryFat + readSector, gbuffer);
            readSector += FAT_READ_SECTOR_COUNT;
        }
    }

    if (j != 0) writeBlockDisk(entryFat + readSector, gbuffer);
    fileCurrent->fileBody.clusterChain = nextClusterChain;
    fileCurrent->fileBody.ptrCurFile += asize;
    fileCurrent->fileBody.phisicalEntryFat = entryFat;
    fileCurrent->fileBody.sectorCount = readSector;
    fileCurrent->fileBody.indexFile = j;
     
    return asize;
}

void Cfat::setCurrentFileSize(u32 asize) {
    u32 index = fileCurrent->fileHead.index, readSector = fileCurrent->fileHead.sectorCount;
    u32 entryFat = fileCurrent->fileHead.phisicalEntryFat;
    readBlockDisk(entryFat + readSector, gbuffer);
    SdirFat32 *dirFat = (SdirFat32*) (gbuffer);
    u32 initialized = dirFat[index].fstClusLO | dirFat[index].fstClusHI << 16;

    if (initialized == 0) { //se inicializa el fichero  
        u32 newEntryFat = getEntryFatFree(END_CHAIN_F);
        dirFat[index].fstClusHI = u16(newEntryFat >> 16);
        dirFat[index].fstClusLO = u16(newEntryFat);
        fileCurrent->fileBody.phisicalEntryFat = getPhisicalBlockData(newEntryFat);
        fileCurrent->fileBody.clusterChain = newEntryFat;
        fileCurrent->fileBody.entryFat = newEntryFat;
    }
    dirFat[index].fileSize += asize;
    fileCurrent->fileSize += asize;
    writeBlockDisk(entryFat + readSector, gbuffer);
}

void Cfat::initializeFile() {
    fileCurrent->fileBody.clusterChain = 0;
    fileCurrent->fileBody.ptrCurFile = 0;
    fileCurrent->fileBody.phisicalEntryFat = 0;
    fileCurrent->fileBody.sectorCount = 0;
    fileCurrent->fileBody.indexFile = 0;
    fileCurrent->fileHead.sectorCount = 0;
    fileCurrent->fileHead.phisicalEntryFat = 0;
    fileCurrent->fileHead.index = 0;
}

u32 Cfat::setPosFile(int apos) {
    u32 size = fatDescriptor.bytesXSector*FAT_READ_SECTOR_COUNT, j = 0;
    u32 nextClusterChain, entryFat, pos = 0, sectorCount = 0;
    if (scommand == SEEK_CUR_FILE) {
        if ((fileCurrent->fileBody.ptrCurFile + apos) > fileCurrent->fileSize) return 0;
        if ((fileCurrent->fileBody.ptrCurFile + apos) < 0) return 0;
        if (apos < 0) {
            nextClusterChain = fileCurrent->fileBody.entryFat;
            entryFat = getPhisicalBlockData(fileCurrent->fileBody.entryFat);
            apos = fileCurrent->fileBody.ptrCurFile + apos;
            fileCurrent->fileBody.ptrCurFile = 0;
        } else {
            pos = fileCurrent->fileBody.ptrCurFile;
            sectorCount = fileCurrent->fileBody.sectorCount;
            nextClusterChain = fileCurrent->fileBody.clusterChain;
            entryFat = fileCurrent->fileBody.phisicalEntryFat;
        }
    } else if (scommand == SEEK_SET_FILE) {
        if (apos > fileCurrent->fileSize) return 0;
        nextClusterChain = fileCurrent->fileBody.entryFat;
        entryFat = getPhisicalBlockData(fileCurrent->fileBody.entryFat);
        fileCurrent->fileBody.ptrCurFile = 0;
    } else if (scommand == SEEK_END_FILE) {
        pos = fileCurrent->fileSize;
        sectorCount = fileCurrent->fileBody.sectorCount;
        nextClusterChain = fileCurrent->fileBody.clusterChain;
        entryFat = fileCurrent->fileBody.phisicalEntryFat;
    }
    pos += apos;
    sectorCount += pos / size;
    u32 index = pos % size;
    u32 clusterCount = sectorCount / fatDescriptor.sectorXCluster;
    u32 readSector = sectorCount % fatDescriptor.sectorXCluster;

    while (j < clusterCount) {
        entryFat = nextClusterChainDir(&nextClusterChain);
        if (entryFat == 0)return 0;
        j++;
    }
    fileCurrent->fileBody.clusterChain = nextClusterChain;
    fileCurrent->fileBody.ptrCurFile += apos;
    fileCurrent->fileBody.phisicalEntryFat = entryFat;
    fileCurrent->fileBody.sectorCount = readSector;
    fileCurrent->fileBody.indexFile = index;
    return fileCurrent->fileBody.ptrCurFile;
}

void Cfat::closeFile() {
    if (fileCurrent) delete(fileCurrent);
    if (dirCurrent) delete(dirCurrent);
    if (fileInfo) delete(fileInfo);
    fileInfo = 0;
    fileCurrent = 0;
    dirCurrent = 0;
}