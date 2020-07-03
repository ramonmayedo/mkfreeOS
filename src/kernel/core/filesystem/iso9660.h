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

#ifndef ISO9660_H
#define ISO9660_H

#include "includes/hiso9660.h"

#define ISO_ENTRY_ERROR_DIR0 0x45   //Las definiciones para ISO son a partir de 0x40

class Ciso9660 {
public:
    Ciso9660(void *aiso9660Structure, int aidDisk, int aoffsetSectorPartition);
    void setDirPtr(void *aDirPtr);
    void *getDirPtr();
    void setFilePtr(void *aFilePtr);
    void *getFilePtr();
    int command(int acommand, char * astring);
    int commandFile(int acommand, char *astring, u32 asize, u32 aptr);
    SfileInfo* getFileInfo();
    void closeFile();
    SinfoMedia *getInfoVolume();
private:
    int idDisk; //numero de dispositivo
    u32 scommand;
    char *gbuffer;
    char *gname;
    SfileInfo *fileInfo, rootInfo;
    SdirOpenIso9660 *dirCurrent;
    SfileOpenIso9660 *fileCurrent;
    Siso9660Descriptor iso9660Descriptor;
    SinfoMedia infoMedia;
    int setDir(char *adir, u32 *aEntryBlock);
    int getNextFile();
    int dirExist(char *filename, u8 size, ScahePathSearch *cahePathSearch, int endFound);
    int fileExist(char *filename, u8 size, ScahePathSearch *cahePathSearch);
    int isValidChar(char *acar);
    void openFilePtr(char *fileName, ScahePathSearch *cahePathSearch);
    void openDirPtr(char *fileName, ScahePathSearch *cahePathSearch);
    int readFile(char *abuffer, u32 asize);
    u32 setPosFile(u32 apos, u32 result);
    int readBlockDisk(u32 ablock, char *abuffer, int nBlock);
    int writeBlockDisk(u32 ablock, char *abuffer, int nBlock);
    int getKernelFlagsFile(int flagas);
    int initDirOpen();
    void getRootInfo();
};

#endif

