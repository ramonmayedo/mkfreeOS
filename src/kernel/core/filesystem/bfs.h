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

#ifndef BFS_H
#define BFS_H
#include "includes/hbfs.h"
#include "../filesystem.h"


class Cbfs {
public:
    Cbfs(void *abfsStrcture, int aidDisk, int aoffsetSectorPartition);
    int command(int acommand, char * astring);
    int commandFile(int acommand, char *astring, u32 asize, u32 aptr);
    int command(int acommand, char * astring1, char * astring2);
    SinfoMedia *getInfoVolume();
    SfileInfo* getFileInfo();
    void *getFilePtr();
    void *getDirPtr();
    void setFilePtr(void *aFilePtr);
    void setDirPtr(void *aDirPtr);
    void closeFile();
protected:
    int  setDir(char *adir,u32 *aEntryBlock);
    int initDirOpen();
    int isValidName(char *aname);
    int isValidChar(char acar);
    int fileExist(u32 *aEntryBlock, char *aname, int acceptHandle);
    int setFileInfo(SblockData *blockData,SblockData *blockDataDir,int index,int entryBlock);
    int getKernelFlagsFile(int flagas);
    int readBlockDisk(u32 ablock, char *abuffer,int nBlock);
    int Cbfs::writeBlockDisk(u32 ablock, char *abuffer,int nBlock);
    int readFile(char *abuffer, u32 asize);
    int readNextFile();
    int writeFile(char *abuffer, u32 asize);
    int newFile(char *anameFile, int atypeFile);
    int getEntryBlock(int offsetMap);
    void setCurrentFileSize(u32 asize,u32 countCluster);
    u32 newClusterFile(char *buffer,u32 entryBlock);
    u32 setPosFile(u32 apos,u32 result);
    int deleteFile(u32 entryBlock,bool delEntryParent);
    int deleteFolder(u32 entryBlock);
    int deleteFileFolder(char *anameFile);
    int renameFile(char *nameFile, char *newNameFile);
    int moveFile(char *src, char *dest);
    int getEntryFree(SblockData *blockData,int type,u32 entryBlock);
private:
    int idDisk;    //numero de dispositivo
    int scommand;
    char *gname;
    char *gbuffer;
    char *ebuffer;
    u32 sizeBuff;
    u32 dataSectorStart;
    u32 bitMap;
    u32 acceptHandle;
    SbfsDescriptor bfsDescriptor; 
    SinfoMedia infoMedia;
    SfileInfo *fileInfo, rootInfo; //Informacion del directorio raiz o cualquier archivo abierto
    SdirOpenBfs *dirCurrent; //Directorio actual abierto
    SfileOpenBfs *fileCurrent; //Archivo actual abierto
};

#endif

