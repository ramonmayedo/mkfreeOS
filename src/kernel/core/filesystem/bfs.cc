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

#include "bfs.h"
#include "../../architecture/x86/x86.h"
#include "includes/hfat.h"

extern Sx86 x86;
extern Score core;

Cbfs::Cbfs(void *abfsStrcture, int aidDisk, int aoffsetSectorPartition) {
    SfbsPartition *fbsPartition = (SfbsPartition*) abfsStrcture;
    core.string.strCopy(infoMedia.labelVolume, (char*) fbsPartition->label, sizeof (fbsPartition->label));
    core.string.strCopy(infoMedia.labelType, (char*) fbsPartition->type, sizeof (fbsPartition->type));
    bfsDescriptor.sectorCount = fbsPartition->sectorCount;
    bfsDescriptor.sectorStart = fbsPartition->sectorStart;
    bfsDescriptor.sectorEnd = fbsPartition->sectorEnd;
    bfsDescriptor.sectorXcluster = fbsPartition->sectorXcluster;
    bfsDescriptor.offset = fbsPartition->offset;
    bfsDescriptor.byteXSector = fbsPartition->byteXSector;
    infoMedia.size = (bfsDescriptor.sectorCount + bfsDescriptor.offset) * bfsDescriptor.byteXSector;
    dataSectorStart = aoffsetSectorPartition + bfsDescriptor.sectorStart;
    bitMap = aoffsetSectorPartition + 2;
    gname = new char[256];
    gbuffer = new char[512 * bfsDescriptor.sectorXcluster];
    ebuffer = new char[512 * bfsDescriptor.sectorXcluster];
    idDisk = aidDisk;
    fileInfo = 0;
    sizeBuff = 512 * bfsDescriptor.sectorXcluster;
}

int Cbfs::command(int acommand, char * astring) {
    scommand = acommand;
    u32 entry = dataSectorStart;
    switch (acommand) {
        case FILE_EXIST : return setDir(astring, &entry);
        case OPEN_DIR: return setDir(astring, &entry);
        case OPEN_FILE: return setDir(astring, &entry);
        case NEW_FOLDER: return newFile(astring, ATTR_DIRECTORY_BFS);
        case NEW_FILE: return newFile(astring, ATTR_ARCHIVE_BFS);
        case DELETE_FILE:return deleteFileFolder(astring);
        case DELETE_FOLDER:return deleteFileFolder(astring);
        default: return COMMAND_INVALID;
    }
}

int Cbfs::commandFile(int acommand, char *astring, u32 asize, u32 aptr) {
    scommand = acommand;
    u32 size = asize*aptr;
    switch (acommand) {
        case READ_FILE: return readFile(astring, size); //fija la ruta a partir de la raiz
        case WRITE_FILE: return writeFile(astring, size);
        case SEEK_CUR_FILE: return setPosFile(asize,aptr);
        case SEEK_SET_FILE: return setPosFile(asize,aptr);
        case SEEK_END_FILE: return setPosFile(asize,aptr);

        case GET_NEXT_FILE:
        {
            int error = fileExist(&dirCurrent->phisicalEntryBlock, astring, 1);
            if (error == 0) return 1;
            else return 0;
        }
        default: return COMMAND_INVALID;
    }
}

int Cbfs::command(int acommand, char * astring1, char * astring2) {
    scommand = acommand;
    switch (acommand) {
        case RENAME_FILE: return renameFile(astring1, astring2);
        case MOVE_FILE:   return moveFile(astring1, astring2);
        default: return COMMAND_INVALID;
    }
}

int Cbfs::setDir(char *adir,u32 *aEntryBlock) {
    int maxLenghtDirPath = core.string.strLenght(adir); //tamaño de la ruta

    if (maxLenghtDirPath == 0) {
        if (scommand == OPEN_FILE) return FILE_NO_EXIST;
        if (scommand == OPEN_DIR) return initDirOpen();
        return FILE_NO_EXIST;
    }
    acceptHandle = 0;
    int error = 0, ptrIndex = 0;
    for (int i = 0; i <= maxLenghtDirPath; i++) {
        if (adir[i] != '/' && i != maxLenghtDirPath) {
            gname[ptrIndex] = adir[i];
            ptrIndex++;
            if (ptrIndex == 238) return NAME_MAX_LENGHT;
        } else {
            gname[ptrIndex] = 0x0;
            if (error = isValidName(gname)) return error;
            if (i == maxLenghtDirPath) acceptHandle = FIND_END;
            if (error = fileExist(aEntryBlock, gname, acceptHandle)) return error;
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

int Cbfs::fileExist(u32 *aEntryBlock, char *aname, int acceptHandle) {
    u32 entryBlock = *aEntryBlock; 
    int i = 0;
    if (scommand == GET_NEXT_FILE) {
        entryBlock = dirCurrent->entryBlock;
        i = dirCurrent->indexFile;
        dirCurrent->indexFile++;
    }
    readBlockDisk(entryBlock, gbuffer, bfsDescriptor.sectorXcluster);
    SblockData * blockData;// = new SblockData;
    SblockData * blockDataDir;// = new SblockData;
    blockData = (SblockData*) gbuffer;
    for (i; i < 224; i++) {
        if (blockData->entry[i].blockData != 0) {
            readBlockDisk(blockData->entry[i].blockData, ebuffer, 1);
            blockDataDir = (SblockData*) ebuffer;
            if (scommand == GET_NEXT_FILE) return setFileInfo(blockData, blockDataDir, i, entryBlock);               
            if (core.string.strCmp(aname, blockDataDir->name, core.string.strLenght(blockDataDir->name)) == 0) {
                if (acceptHandle == FIND_END) setFileInfo(blockData, blockDataDir, i, entryBlock);
                *aEntryBlock = blockData->entry[i].blockData;
                return 0;
            }
        }
    }
    return FILE_NO_EXIST;
}

int Cbfs::newFile(char *anameFile, int atypeFile) {
    u32 EntryBlock = dataSectorStart;
    int error;
    if (setDir(anameFile, &EntryBlock) == 0) return FILE_EXIST;

    if ((error = isValidName(gname))) return error;
    u32 newEntry = getEntryBlock(bitMap);

    char *ptr = ebuffer;
    for (int i = 0; i < sizeBuff; i++, ptr++) *ptr = 0x0;

    readBlockDisk(EntryBlock, gbuffer, bfsDescriptor.sectorXcluster);
    SblockData * blockData = (SblockData*) gbuffer;
    SblockData * blockDataNew = (SblockData*) ebuffer;
    for (int i = 0; i < 224; i++) { //Busco alguna entrada libre
        if (blockData->entry[i].type == 0) {
            blockData->entry[i].type = atypeFile;
            blockData->entry[i].blockData = newEntry;
            blockDataNew->countCluster = 0;
            blockDataNew->fileType = atypeFile;
            blockDataNew->parent = EntryBlock;
            blockDataNew->indexEntry = i;
            blockDataNew->size = 0;
            blockDataNew->typeBlock.type = BLOCK_NODE;
            blockDataNew->typeBlock.blockData = 0;
            core.string.strCopy(blockDataNew->name, gname, core.string.strLenght(gname));
            writeBlockDisk(EntryBlock, gbuffer, bfsDescriptor.sectorXcluster);
            writeBlockDisk(newEntry, ebuffer, bfsDescriptor.sectorXcluster);
            return 0;
        }
    }
    return DIRECTORY_ENTRY_FULL;
}


int Cbfs::getEntryBlock(int offsetMap) {
    readBlockDisk(offsetMap, gbuffer, bfsDescriptor.sectorXcluster);

        for (int i = 0; i < sizeBuff; i++)
            for (int j = 0; j < 8; j++) {
                if ((gbuffer[i] & (1 << j)) == 0) {
                    gbuffer[i] = gbuffer[i] | (1 << j);
                    writeBlockDisk(offsetMap, gbuffer, bfsDescriptor.sectorXcluster);
                    return (i * 8 + j)*4 + offsetMap + 4;
                }
            }
    return 0;
}


int Cbfs::isValidChar(char acar) {
    char format[18] = {'!', '#', '$', '%', '&', '\'', '(', ')', '-', '@', '^', '_', '`', '{', '}', '~', ' ', '.'};
    for (int j = 0; j < 18; j++)
        if (acar == format[j]) return 1;
    return 0;
}

int Cbfs::isValidName(char *aname) {
    int sizeName = core.string.strLenght(aname); //tamaño del nomber
    if (sizeName > 240) return NAME_MAX_LENGHT; //si tiene mas 240 error
    char car;
    for (int i = 0; i < sizeName; i++) {
        car = aname[i];
        if ((car >= 'a' && car <= 'z' || car >= 'A' && car <= 'Z' || car >= '0' && car <= '9' || isValidChar(car))) {
        } else return NAME_INVALID;
    }
    return 0;
}

int Cbfs::initDirOpen() {
    dirCurrent = new SdirOpenBfs;
    dirCurrent->entryBlock = dataSectorStart;
    dirCurrent->phisicalEntryBlock = dataSectorStart;
    dirCurrent->indexFile = 0;
    dirCurrent->entryNextFile = 0;
    return 0;
}

int Cbfs::getKernelFlagsFile(int flagas) {
    if (flagas == ATTR_DIRECTORY_BFS) return ATTRK_DIRECTORY;
    if (flagas == ATTR_ARCHIVE_BFS) return ATTRK_ARCHIVE;
    return flagas;
}

int Cbfs::setFileInfo(SblockData *blockData, SblockData *blockDataDir, int index, int entryBlock) {
    if (fileInfo == 0) fileInfo = new SfileInfo;
    fileInfo->fileSize = blockDataDir->size;
    fileInfo->typeFile = getKernelFlagsFile(blockDataDir->fileType);
    core.string.strCopy(fileInfo->name, blockDataDir->name, core.string.strLenght(blockDataDir->name));

    if (scommand == OPEN_FILE) {
        fileCurrent = new SfileOpenBfs; //Se crea el archivo
        fileCurrent->fileHead.phisicalEntryBlock = entryBlock;
        fileCurrent->fileHead.entryBlock = blockData->entry[index].blockData;
        fileCurrent->fileBody.phisicalEntryBlock = blockData->entry[index].blockData;
        fileCurrent->fileBody.indexBlock = 256;
        fileCurrent->fileBody.ptrCurFile = 0;
        fileCurrent->fileBody.entryNextBlock = blockDataDir->typeBlock.blockData;
        fileCurrent->fileSize = blockDataDir->size;
    } else if (scommand == OPEN_DIR) {
        dirCurrent = new SdirOpenBfs;
        dirCurrent->entryBlock = blockData->entry[index].blockData;
        dirCurrent->phisicalEntryBlock = blockData->entry[index].blockData;
        dirCurrent->indexFile = 0;
        dirCurrent->entryNextFile = 0;
    }
    return 0;
}

int Cbfs::readBlockDisk(u32 ablock, char *abuffer,int nBlock) {
    return core.cacheDisk.readSector(idDisk, ablock, nBlock, abuffer);
}

int Cbfs::writeBlockDisk(u32 ablock, char *abuffer,int nBlock) {
    return core.cacheDisk.writeSector(idDisk, ablock, nBlock, abuffer);
}

void *Cbfs::getFilePtr() {
    return (void*) fileCurrent;
}

void *Cbfs::getDirPtr() {
    return (void*) dirCurrent;
}

void Cbfs::setFilePtr(void *aFilePtr) {
    fileCurrent = (SfileOpenBfs*) aFilePtr;
}

void Cbfs::setDirPtr(void *aDirPtr) {
    dirCurrent = (SdirOpenBfs*) aDirPtr;
}

SinfoMedia *Cbfs::getInfoVolume() {
    return &infoMedia;
}

SfileInfo* Cbfs::getFileInfo() {
    return fileInfo;
}

void Cbfs::closeFile() {
    if (fileCurrent) delete(fileCurrent);
    if (dirCurrent) delete(dirCurrent);
    if (fileInfo) delete(fileInfo);
    fileInfo = 0;
    fileCurrent = 0;
    dirCurrent = 0;
}
// -----------------------------Manejo de Archivos------------------------------

int Cbfs::readFile(char *abuffer, u32 asize) {
    if (fileCurrent->fileBody.ptrCurFile >= fileCurrent->fileSize) return 0;
    u32 entryBlock = fileCurrent->fileBody.phisicalEntryBlock;
    u32 j = fileCurrent->fileBody.indexBlock, i = 0,size = asize;    
    if (fileCurrent->fileBody.ptrCurFile + asize > fileCurrent->fileSize) size = fileCurrent->fileSize - fileCurrent->fileBody.ptrCurFile;
    if (j != 8) readBlockDisk(entryBlock, gbuffer, bfsDescriptor.sectorXcluster);
    while (i < size) {
        if (j == 8) {
            entryBlock = fileCurrent->fileBody.entryNextBlock;
            readBlockDisk(entryBlock, gbuffer, bfsDescriptor.sectorXcluster);
            fileCurrent->fileBody.entryNextBlock = ((SentryBlock*) gbuffer)->blockData;
        }
        abuffer[i] = gbuffer[j];
        j++;
        i++;
        if (j == sizeBuff) j = 8;
    }
    fileCurrent->fileBody.ptrCurFile += size;
    fileCurrent->fileBody.phisicalEntryBlock = entryBlock;
    fileCurrent->fileBody.indexBlock = j;

    return size;
}

int Cbfs::writeFile(char *abuffer, u32 asize) {
    u32 entryBlock = fileCurrent->fileBody.phisicalEntryBlock;
    u32 j = fileCurrent->fileBody.indexBlock, i = 0, countCluster = 0;
    
    readBlockDisk(entryBlock, ebuffer, bfsDescriptor.sectorXcluster);
    while (i < asize) {
        if (j == 8) {
            fileCurrent->fileBody.entryNextBlock = ((SblockDataEntryFile*) ebuffer)->typeBlock.blockData;
            if (fileCurrent->fileBody.entryNextBlock == 0) {
                countCluster++;
                entryBlock = newClusterFile(ebuffer, entryBlock);
                if (entryBlock == 0) return 0;
            } else {
                writeBlockDisk(entryBlock, ebuffer, bfsDescriptor.sectorXcluster);
                entryBlock = fileCurrent->fileBody.entryNextBlock;
                readBlockDisk(entryBlock, ebuffer, bfsDescriptor.sectorXcluster);
            }
        }
        ebuffer[j] = abuffer[i];
        j++;
        i++;
        if (j == sizeBuff) j = 8;
    }
    writeBlockDisk(entryBlock, ebuffer, bfsDescriptor.sectorXcluster);
    if ((fileCurrent->fileBody.ptrCurFile + asize) > fileCurrent->fileSize) setCurrentFileSize(asize, countCluster);
    fileCurrent->fileBody.ptrCurFile += asize;
    fileCurrent->fileBody.phisicalEntryBlock = entryBlock;
    fileCurrent->fileBody.indexBlock = j;
    return 0;
}

void Cbfs::setCurrentFileSize(u32 asize, u32 countCluster) {
    u32 entryBlock = fileCurrent->fileHead.entryBlock;
    readBlockDisk(entryBlock, ebuffer, bfsDescriptor.sectorXcluster);
    SblockData * blockData = (SblockData*) ebuffer;
    blockData->size += asize;
    blockData->countCluster += countCluster;
    fileCurrent->fileSize = blockData->size;
    writeBlockDisk(entryBlock, ebuffer, bfsDescriptor.sectorXcluster);
}

u32 Cbfs::newClusterFile(char *buffer, u32 entryBlock) {
    u32 newEntry = getEntryBlock(bitMap); //Se obtiene un nuevo cluster 
    if (newEntry == 0) return 0;
    ((SblockData*) buffer)->typeBlock.blockData = newEntry;
    writeBlockDisk(entryBlock, buffer, bfsDescriptor.sectorXcluster); //Se escribe el cluster ya lleno
    char *ptr = buffer;
    for (int k = 0; k < sizeBuff; k++, ptr++) *ptr = 0x0;
    ((SblockDataEntryFile*) buffer)->typeBlock.blockData = 0;
    ((SblockDataEntryFile*) buffer)->typeBlock.type = BLOCK_DATA_FILE;
    return newEntry;
}

u32 Cbfs::setPosFile(u32 apos, u32 result) {
    u32 entryBlock = fileCurrent->fileHead.entryBlock,j = 256, i = 0, pos;

    if (scommand == SEEK_SET_FILE) {                               //Si es a partir del comienzo 
        pos = (int) apos;
    } else if (scommand == SEEK_CUR_FILE) {                        //Cursor relativo
        pos = (int) fileCurrent->fileBody.ptrCurFile + (int) apos; //Suma relativa
    } else if (scommand == SEEK_END_FILE) {
        pos = (int) fileCurrent->fileSize + (int) apos;
    }

    if (pos > fileCurrent->fileSize) pos = fileCurrent->fileSize; //Si es mayor va al final   
    *(int*)result = pos;
    if (pos == fileCurrent->fileBody.ptrCurFile) return 0;

    readBlockDisk(entryBlock, gbuffer, bfsDescriptor.sectorXcluster);
    fileCurrent->fileBody.entryNextBlock = ((SblockData*) gbuffer)->typeBlock.blockData;
    while (i < pos) {
        if (j == 8) {
            entryBlock = fileCurrent->fileBody.entryNextBlock;
            readBlockDisk(entryBlock, gbuffer, bfsDescriptor.sectorXcluster);
            fileCurrent->fileBody.entryNextBlock = ((SblockData*) gbuffer)->typeBlock.blockData;
        }
        j++;
        i++;
        if (j == sizeBuff) j = 8;
    }
    fileCurrent->fileBody.ptrCurFile = pos;
    fileCurrent->fileBody.phisicalEntryBlock = entryBlock;
    fileCurrent->fileBody.indexBlock = j;

    return 0;
}

int Cbfs::deleteFileFolder(char *anameFile) {
    u32 entryBlock = dataSectorStart;
    if (setDir(anameFile, &entryBlock)) return FILE_NO_EXIST;

    if (scommand == DELETE_FILE) {
        if (((SblockData*) ebuffer)->fileType == ATTR_ARCHIVE_BFS)
            return deleteFile(entryBlock, true);
    } else if (scommand == DELETE_FOLDER) {
        if (((SblockData*) ebuffer)->fileType == ATTR_DIRECTORY_BFS)
            return deleteFolder(entryBlock);
    }
    return FILE_NO_EXIST;
}

int Cbfs::deleteFile(u32 entryBlock,bool delEntryParent) {
    readBlockDisk(bitMap, gbuffer, bfsDescriptor.sectorXcluster);
    readBlockDisk(entryBlock, ebuffer, bfsDescriptor.sectorXcluster);
    int offset = (entryBlock - bitMap - 4) / 4;
    gbuffer[offset / 8] ^= (1 << (offset % 8));

    u32 nexCluster = ((SblockData*) ebuffer)->typeBlock.blockData;
    //Se borra la entrada en el directorio padre
    if (delEntryParent) {
        u32 indexEntry = ((SblockData*) ebuffer)->indexEntry;
        u32 blockActual = ((SblockData*) ebuffer)->parent;
        readBlockDisk(blockActual, ebuffer, bfsDescriptor.sectorXcluster);
        ((SblockData*) ebuffer)->entry[indexEntry].blockData = 0x0;
        ((SblockData*) ebuffer)->entry[indexEntry].type = 0x0;
        writeBlockDisk(blockActual, ebuffer, bfsDescriptor.sectorXcluster);
    }
    //Se liberan todos los cluster asignados al archivo
    while (nexCluster) {
        offset = (nexCluster - bitMap - 4) / 4;
        if (gbuffer[offset / 8] & (1 << offset % 8))
            gbuffer[offset / 8] ^= (1 << (offset % 8));
        else return 0;
        readBlockDisk(nexCluster, ebuffer, bfsDescriptor.sectorXcluster);
        nexCluster = ((SblockData*) ebuffer)->typeBlock.blockData;
    }
    writeBlockDisk(bitMap, gbuffer, bfsDescriptor.sectorXcluster);
    return 0;
}

int Cbfs::deleteFolder(u32 entryBlock) {
    readBlockDisk(entryBlock, ebuffer, bfsDescriptor.sectorXcluster);
    SblockData *data = (SblockData*) ebuffer;
    for (int i = 0; i < 224; i++) {
        if (data->entry[i].type != 0) {
            if (data->entry[i].type == ATTR_DIRECTORY_BFS) deleteFolder(data->entry[i].blockData);
            else deleteFile(data->entry[i].blockData, false);
            readBlockDisk(entryBlock, ebuffer, bfsDescriptor.sectorXcluster);
            data->entry[i].blockData = 0x0;
            data->entry[i].type = 0x0;
            writeBlockDisk(entryBlock, ebuffer, bfsDescriptor.sectorXcluster);
        }
    }
    deleteFile(entryBlock, true);
    return 0;
}

int Cbfs::renameFile(char *nameFile, char *newNameFile) {
    u32 entryBlock = dataSectorStart;
    if (setDir(nameFile, &entryBlock)) return FILE_NO_EXIST;
    int error;
    if ((error = isValidName(newNameFile))) return error;
    core.string.strCopy(((SblockData*) ebuffer)->name, newNameFile, core.string.strLenght(newNameFile));
    writeBlockDisk(entryBlock, ebuffer, bfsDescriptor.sectorXcluster);
    return 0;
}

int Cbfs::moveFile(char *src, char *dest) {
    if (core.string.strCmp(src,dest,core.string.strLenght(src)) == 0) return 0; //src = dest
    u32 entryBlockSrc = dataSectorStart;
    if (setDir(src, &entryBlockSrc)) return FILE_NO_EXIST;
    u32 blockParent = ((SblockData*) ebuffer)->parent;
    u32 indexParent = ((SblockData*) ebuffer)->indexEntry;
    int type = ((SblockData*) ebuffer)->fileType;

    u32 entryBlockDest = dataSectorStart;
    if (setDir(dest, &entryBlockDest)==0)
        return FILE_EXIST;                          //Ya existe un fichero con ese nombre
    if (acceptHandle == 0) return FILE_NO_EXIST;    //No existe la ruta de destino
   
    int error;
    if ((error = isValidName(gname))) return error;   //Es valido el nombre

    if (blockParent == entryBlockDest) {//Si se mueve a la misma ruta solo se cambia el nombre
        readBlockDisk(entryBlockSrc, ebuffer, bfsDescriptor.sectorXcluster);
        core.string.strCopy(((SblockData*) ebuffer)->name, gname, core.string.strLenght(gname));
        writeBlockDisk(entryBlockSrc, ebuffer, bfsDescriptor.sectorXcluster);   
        return 0;
    }
    
    int indexDest = getEntryFree(((SblockData*) gbuffer), type, entryBlockSrc);
    writeBlockDisk(entryBlockDest, gbuffer, bfsDescriptor.sectorXcluster);
    //Nueva entrada en el destino
    readBlockDisk(blockParent, ebuffer, bfsDescriptor.sectorXcluster);
    ((SblockData*) ebuffer)->entry[indexParent].blockData = 0x0;
    ((SblockData*) ebuffer)->entry[indexParent].type = 0x0;
    writeBlockDisk(blockParent, ebuffer, bfsDescriptor.sectorXcluster);
    //Se modifica la entrada de la fuente
    readBlockDisk(entryBlockSrc, ebuffer, bfsDescriptor.sectorXcluster);
    core.string.strCopy(((SblockData*) ebuffer)->name, gname, core.string.strLenght(gname));
    ((SblockData*) ebuffer)->parent = entryBlockDest;
    ((SblockData*) ebuffer)->indexEntry = indexDest;
    writeBlockDisk(entryBlockSrc, ebuffer, bfsDescriptor.sectorXcluster);
    
    return 0;
}

int Cbfs::getEntryFree(SblockData *blockData, int type, u32 entryBlock) {
    for (int i = 0; i < 224; i++) { //Busco alguna entrada libre
        if (blockData->entry[i].type == 0) {
            blockData->entry[i].type = type;
            blockData->entry[i].blockData = entryBlock;
            return i;
        }
    }
    return -1;
}