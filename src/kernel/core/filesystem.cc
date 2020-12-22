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

#include "filesystem.h"
#include "../filesystem/fat.h"
#include "../filesystem/bfs.h"
#include "../architecture/x86/x86.h"
#include "../filesystem/iso9660.h"

extern Sx86 x86;
extern Score core;
CfileSystem::CfileSystem() {
    mediaOcuped = 0;
    mediaFree = MAX_UNI;
    indexFree = 0;
}

int CfileSystem::mountFileSystem(int aid, void *astrcture, int aidDisk, int aoffsetSectorPartition) {
    if (aid == MEDIA_FAT32) {
        Cfat *fat = new Cfat(astrcture, aidDisk, aoffsetSectorPartition);
        media[indexFree].flags |= MEDIA_BUSY;
        media[indexFree].letter = ('a' + indexFree);
        media[indexFree].type = MEDIA_FAT32;
        media[indexFree].structFileSystem = (void*) fat;
        media[indexFree].idDisk = aidDisk;
        media[indexFree].aoffsetSectorPartition = aoffsetSectorPartition;
        indexFree++;
    } else if (aid == BFS) {
        Cbfs *bfs = new Cbfs(astrcture, aidDisk, aoffsetSectorPartition);
        media[indexFree].flags |= MEDIA_BUSY;
        media[indexFree].letter = ('a' + indexFree);
        media[indexFree].type = BFS;
        media[indexFree].structFileSystem = (void*) bfs;
        media[indexFree].idDisk = aidDisk;
        media[indexFree].aoffsetSectorPartition = aoffsetSectorPartition;
        indexFree++;
    } else if (aid == ISO9660) {
        Ciso9660 *iso9660 = new Ciso9660(astrcture, aidDisk, aoffsetSectorPartition);
        media[indexFree].flags |= MEDIA_BUSY;
        media[indexFree].letter = ('a' + indexFree);
        media[indexFree].type = ISO9660;
        media[indexFree].structFileSystem = (void*) iso9660;
        media[indexFree].idDisk = aidDisk;
        media[indexFree].aoffsetSectorPartition = aoffsetSectorPartition;
        indexFree++;
    }
    return aid;
}

SinfoMedia *CfileSystem::getInfoVolume() {
    if (media[indexMedia].type == MEDIA_FAT32)
        return ((Cfat*) media[indexMedia].structFileSystem)->getInfoVolume();
    else if (media[indexMedia].type == BFS)
        return ((Cbfs*) media[indexMedia].structFileSystem)->getInfoVolume();
   else if (media[indexMedia].type == ISO9660)
        return ((Ciso9660*) media[indexMedia].structFileSystem)->getInfoVolume(); 
}

int CfileSystem::getIndexMedia(char aletter) {
    for (int i = 0; i < MAX_UNI; i++) {
        if ((media[i].flags & MEDIA_BUSY) && media[i].letter == aletter)
            return i;
    }
    return -1;
}

SfileInfo* CfileSystem::getFileInfo() {
    if (media[indexMedia].type == MEDIA_FAT32)
        fileInfo = ((Cfat*) media[indexMedia].structFileSystem)->getFileInfo();
    else if (media[indexMedia].type == BFS)
        fileInfo = ((Cbfs*) media[indexMedia].structFileSystem)->getFileInfo();
    else if (media[indexMedia].type == ISO9660)
        fileInfo = ((Ciso9660*) media[indexMedia].structFileSystem)->getFileInfo();
    return fileInfo;
}

int CfileSystem::selectVolume(char aletter) {
    indexMedia = getIndexMedia(aletter);
    return indexMedia;
}

int CfileSystem::commandFile(int acommand, char *astring, u32 asize, u32 aptr) {
    if (media[indexMedia].type == MEDIA_FAT32)
        error = ((Cfat*) media[indexMedia].structFileSystem)->commandFile(acommand, astring, asize, aptr);
    else if (media[indexMedia].type == BFS)
        error = ((Cbfs*) media[indexMedia].structFileSystem)->commandFile(acommand, astring, asize, aptr);
    else if (media[indexMedia].type == ISO9660)
        error = ((Ciso9660*) media[indexMedia].structFileSystem)->commandFile(acommand, astring, asize, aptr);
    else return 0;

    return error;
}

int CfileSystem::command(int acommand, char *astring1, char *astring2) {
    selectVolume(astring1[0]);
    if (indexMedia == -1 || astring1[1] != '/') return -1;
    if (media[indexMedia].type == BFS)
        error = ((Cbfs*) media[indexMedia].structFileSystem)->command(acommand, astring1 + 2, astring2 + 2);
    else return 0;

    return error;
}

int CfileSystem::command(int acommand, char *astring) {
    selectVolume(astring[0]);
    if (indexMedia == -1 || astring[1] != '/') return -1;
    if (media[indexMedia].type == MEDIA_FAT32) {
        error = ((Cfat*) media[indexMedia].structFileSystem)->command(acommand, astring + 2);
        return error;
    } else if (media[indexMedia].type == BFS) {
        error = ((Cbfs*) media[indexMedia].structFileSystem)->command(acommand, astring + 2);
        return error;
    } else if (media[indexMedia].type == ISO9660) {
        error = ((Ciso9660*) media[indexMedia].structFileSystem)->command(acommand, astring + 2);   
        return error;
    } else return 0;
}

void *CfileSystem::getFilePtr() {
    if (media[indexMedia].type == MEDIA_FAT32)
        return ((Cfat*) media[indexMedia].structFileSystem)->getFilePtr();
    else if (media[indexMedia].type == BFS)
        return ((Cbfs*) media[indexMedia].structFileSystem)->getFilePtr();
    else if (media[indexMedia].type == ISO9660)
        return ((Ciso9660*) media[indexMedia].structFileSystem)->getFilePtr();
}

void *CfileSystem::getDirPtr() {
    if (media[indexMedia].type == MEDIA_FAT32)
        return ((Cfat*) media[indexMedia].structFileSystem)->getDirPtr();
    else if (media[indexMedia].type == BFS)
        return ((Cbfs*) media[indexMedia].structFileSystem)->getDirPtr();
    else if (media[indexMedia].type == ISO9660)
        return ((Ciso9660*) media[indexMedia].structFileSystem)->getDirPtr();
}

void CfileSystem::setFilePtr(void* afilePtr) {
    if (media[indexMedia].type == MEDIA_FAT32)
        return ((Cfat*) media[indexMedia].structFileSystem)->setFilePtr(afilePtr);
    else if (media[indexMedia].type == BFS)
        return ((Cbfs*) media[indexMedia].structFileSystem)->setFilePtr(afilePtr);
    else if (media[indexMedia].type == ISO9660)
        return ((Ciso9660*) media[indexMedia].structFileSystem)->setFilePtr(afilePtr);
}

void CfileSystem::setDirPtr(void* adirPtr) {
    if (media[indexMedia].type == MEDIA_FAT32)
        return ((Cfat*) media[indexMedia].structFileSystem)->setDirPtr(adirPtr);
    else if (media[indexMedia].type == BFS)
        return ((Cbfs*) media[indexMedia].structFileSystem)->setDirPtr(adirPtr);
    else if (media[indexMedia].type == ISO9660)
        return ((Ciso9660*) media[indexMedia].structFileSystem)->setDirPtr(adirPtr);
}

void CfileSystem::closeFile() {
    if (media[indexMedia].type == MEDIA_FAT32)
        return ((Cfat*) media[indexMedia].structFileSystem)->closeFile();
    else if (media[indexMedia].type == BFS)
        return ((Cbfs*) media[indexMedia].structFileSystem)->closeFile();
    else if (media[indexMedia].type == ISO9660)
        return ((Ciso9660*) media[indexMedia].structFileSystem)->closeFile();
}

int CfileSystem::command(int acommand, int parameter1, int parameter2, int parameter3, int parameter4) {
    Cprocess *process = core.adminProcess.getRun()->process;
    Cfile *file;
    switch (acommand) {
        case cmfCreateDir:
        {
            return command(NEW_FOLDER, (char*) parameter1);
        }
        case cmfDeleteFile:
        {
            return command(DELETE_FILE, (char*) parameter1);
        }
        case cmfDeleteFolder:
        {
            return command(DELETE_FOLDER, (char*) parameter1);
        }
        case cmfRenameFile:
        {
            return command(RENAME_FILE, (char*) parameter1, (char*) parameter2);
        }
        case cmfMoveFile:
        {
            return command(MOVE_FILE, (char*) parameter1, (char*) parameter2);
        }
        case cmfOpen:
        {
            file = new Cfile((char*) parameter1, parameter2);
            if (file->getError() == 0) return process->addFile(file);
            return 0;
        }
        case cmfRead:
        {
            file = (Cfile*) process->getFile(parameter1);
            return file->read(parameter3, (char*) parameter2);
        }
        case cmfWrite:
        {
            file = (Cfile*) process->getFile(parameter1);
            return file->write(parameter3, (char*) parameter2);
        }
        case cmfClose:
        {
            return process->deleteFile(parameter1);
        }
        case cmfLSeek:
        {//    error = sysCall_5(sysFileSystem, cmfLSeek, fd, aoffset, awhence, (int) &result);
            file = (Cfile*) process->getFile(parameter1);
            return file->command(parameter3, parameter2, parameter4);
        }
        case cmfGetEntDir:
        {
            file = (Cfile*) process->getFile(parameter1);
            int nextDir = file->command(GET_NEXT_FILE, 0, 0);
            if (nextDir) {
                core.string.strCopy(((SfileInfo*) parameter2)->name, file->getName(), core.string.strLenght(file->getName()));
                ((SfileInfo*) parameter2)->fileSize = file->getSize();
                ((SfileInfo*) parameter2)->typeFile = file->getTypeFile();
            }
            return nextDir;
        }
            
        default: return 0;
    }
}