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

#ifndef FAT_H
#define FAT_H
#include "includes/hfat.h"

class Cfat {
public:
    Cfat(void *afatStrcture, int aidDisk, int aoffsetSectorPartition);
    int command(int acommand, char * astring);
    int commandFile(int acommand, char *astring, u32 asize, u32 aptr);
    SinfoMedia *getInfoVolume();
    SfileInfo* getFileInfo();
    void *getFilePtr();
    void *getDirPtr();
    void setFilePtr(void *aFilePtr);
    void setDirPtr(void *aDirPtr);
    void closeFile();
    
protected:
    int newFile(char *anameFile, int atypeFile);
    int setDir(char *adir,u32 *aEntryFat);
    int extractFileNameLFN(int aindexDir, int asize, int aoffset, SdirLFNFat32* adirLFNFat, char *aname);
    int fileExistDir(u32 *aEntryFat, char *aname, int acceptHandle);
    int convertNameToShort(char *anameLarge, char *anameShort);
    int getAttrib(int aattrib);
    u32 getEntryFatFree(u32 anewEntryValue);
    int convertToNameLarge(char *anameLarge, SdirLFNFat32 *adirLFNFat32, int aoffset);
    int isValidChar(char acar);
    u8 checkSumCreate(char *aname);
    u16 getTime();
    u16 getDate();
    int isValidName(char *aname);
    int createSubDir(u32 aentryFat, u32 aparentEntry);
    void setAttribFile(SdirFat32 *adirFat, u32 aentryFat, int afileType);
    void getRootInfo();
    void setAttribFileLFN(SdirLFNFat32 *adirLFNFat, int aldirOrd, u8 acheckSum);
    int setFileInfo(SdirFat32 *adirFat, char *name, int aacceptHandle);
    int getEntryDirectoryFree(char *buffer, int apartName, u32 *abeforeEntryDir, int *aindexEntry, u32 *aentryDir); //,int *abeforeentryDir
    u32 setClusterChain(u32 *aentryFat);
    u32 getClusterChain(u32 aentryFat);
    u32 clearCluster(u32 aentryFat, char *abuffer);
    u32 nextClusterChainDir(u32 *anextClusterChain);
    int getPhisicalBlockData(u32 ablockEntry);
    u32 getPhisicalEntryFat(u32 *aindex, u32 aentryFat);
    int readBlockDisk(u32 ablock, char *abuffer);
    int writeBlockDisk(u32 ablock, char *abuffer);
    void initDirOpen();
    int readFile(char *abuffer, u32 asize);
    int writeFile(char *abuffer, u32 asize);
    void setCurrentFileSize(u32 asize);
    u32 setPosFile(u32 apos);
    void initializeFile();
private:
    u32 maxSizeEntryDir; //Maxima cantidad de entradas de directorios por sector 
    u32 maxSizeEntryFat; //Maxima cantidad de entrada a la fat por sector
    u32 fatSectorStart; //Comienzo del sector fisico dentro del disco donde esta la fat
    u32 dataSectorStart; //Comienzo del sector fisico dentro del disco donde esta los datos
    u32 phisicalEntryFatWorkDir;
    u32 sizeBuff; //tamaño del buffer
    int scommand;
    int idDisk; //numero de dispositivo
    char *gname; //puntero para nombre LFN maximo 256 = 255 car + 0x0 fin de cadena
    char *ename;
    char *shortName; //nombre corto
    char *gbuffer; //buffer principal 
    char *ebuffer; //buffer auxiliar
    SfatDescriptor fatDescriptor;
    SinfoMedia infoMedia;
    SfileInfo *fileInfo, rootInfo; //Informacion del directorio raiz o cualquier archivo abierto
    SdirOpen *dirCurrent; //Directorio actual abierto
    SfileOpen *fileCurrent; //Archivo actual abierto
};

#endif 