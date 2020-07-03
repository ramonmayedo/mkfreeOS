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

#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include "../defines/types.h"

#define MAX_UNI       24
#define MEDIA_BUSY    0x1
#define MEDIA_FAT32   0xC

#define ATTRK_DIRECTORY  0x01
#define ATTRK_ARCHIVE    0x02
#define ATTRK_VOLUME_ID  0x04


struct Smedia{
    char  letter;
    int   type;
    char  flags;
    void  *structFileSystem;
    int   idDisk;
    int   aoffsetSectorPartition;
}__attribute__ ((packed));

struct SinfoMedia{
    char  labelVolume[12];
    char  labelType[9];
    char  labelOem[9];
    u32   size;
}__attribute__ ((packed));

struct SfileInfo{
    char name[255];
    int  typeFile;
    u32  fileSize;
    u32  handle;
};

enum commandFileSystem {
    cmfCreateDir = 1, cmfDeleteFile = 2, cmfDeleteFolder = 3, cmfRenameFile = 4, cmfMoveFile = 5,
    cmfOpen = 6, cmfRead = 7, cmfWrite = 8, cmfClose = 9, cmfGetEntDir = 10, cmfLSeek = 11
};

class CfileSystem {
public:
    CfileSystem();
    int mountFileSystem(int aid,void *astructure,int aidDisk,int aoffsetSectorPartition);
    int selectVolume(char aletter);
    int command(int acommand,char *astring);
    int commandFile(int acommand,char *astring,u32 asize,u32 aptr);
    int command(int acommand,char *astring1,char *astring2);
    SinfoMedia *getInfoVolume();
    SfileInfo* getFileInfo();
    void *getFilePtr();
    void *getDirPtr();
    void setFilePtr(void* afilePtr);
    void setDirPtr(void* adirPtr);
    void closeFile();
    int command(int acommand, int parameter1, int parameter2, int parameter3, int parameter4);
private:
    int mediaOcuped;        //Cantidad de medios montadas
    int mediaFree;          //Cantidad de medios disponibles
    int indexFree;          //Index de la tabla de Medios actualmente disponible
    Smedia media[MAX_UNI];  //Tabla de Medios que se pueden montar
    int error;
    SfileInfo *fileInfo;
    int indexMedia;
    
protected:
    int getIndexMedia(char aletter);
};

#endif

