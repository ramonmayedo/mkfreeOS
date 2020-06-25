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

#ifndef FILE_H
#define FILE_H
#include "filesystem.h"
#include "filesystem/includes/commond.h"
//comandos para la manipulacion de los archivos Comun para todos los ssitemas de archivos FAT32, EXT2...


class Cfile {
public:
    Cfile(char *afileName, u32 aflags);
    int read(int asize, char *abuffer);
    int readAll(char *abuffer);
    int write(int asize, char *abuffer);
    int setCursor(int command, int apos,u32 result);
    int getNextFile();
    int getCursor();
    int getSize();
    int getError();
    int command(int acommand, int offset, u32 result);
    void setIdFile(int aidFile);
    int getIdFile();
    char *getName();
    int getTypeFile();
    ~Cfile();
private:
    void *filePtr;
    void *dirPtr;
    char *buffer;
    SfileInfo *info;
    int error;
    int flags;
    int idFile;
};

#endif

