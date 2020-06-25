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

#include "file.h"
#include "../architecture/x86/x86.h"
#include "filesystem/includes/hbfs.h"

extern Score core;
extern Sx86 x86;

Cfile::Cfile(char *afileName, u32 aflags) {
    flags = aflags;
    dirPtr = 0;
    if (flags & FILE_CREATE) { //Si se hay que crearlo
        if (core.fileSystem.command(FILE_EXIST, afileName) == 0) { //Si existe el fichero
            if (flags & FILE_TRUNCATE) //Si exsite la bandera de truncarlo
                error = core.fileSystem.command(DELETE_FILE, afileName); //borro el que exite
            else error = FILE_EXIST; //Error existe el archivo
        }
        if (error == 0) //Si se pudo borrar entonces creo el nuevo
            error = core.fileSystem.command(NEW_FILE, afileName);
        if (error == 0 && (flags & FILE_RW)) //Si no hay error abro el fichero 
            error = core.fileSystem.command(OPEN_FILE, afileName);
    } else if (flags & FILE_RW) //Se abre para lectura o escritura
        error = core.fileSystem.command(OPEN_FILE, afileName);
    else if (flags & DIR_R) {
        error = core.fileSystem.command(OPEN_DIR, afileName);
        if (error == 0) dirPtr = core.fileSystem.getDirPtr();
    } else error = 1;
    
    if (error == 0) {
        filePtr = core.fileSystem.getFilePtr();
        info = core.fileSystem.getFileInfo();
    }
}

int Cfile::read(int asize, char *abuffer) {
    if (error == 0) {
        if (flags & FILE_R) {
            core.fileSystem.setFilePtr(filePtr);
            return core.fileSystem.commandFile(READ_FILE, abuffer, asize, 1);
        }
    }
    return 0;
}

int Cfile::readAll(char *abuffer) {
    if (error == 0) {
        if (flags & FILE_R) {
            core.fileSystem.setFilePtr(filePtr);
            return core.fileSystem.commandFile(READ_FILE, abuffer, info->fileSize, 1);
        }
    }
    return 0;
}

int Cfile::write(int asize, char *abuffer) {
    if (error == 0) {
        if (flags & FILE_W) {
            core.fileSystem.setFilePtr(filePtr);
            return core.fileSystem.commandFile(WRITE_FILE, abuffer, asize, 1);
        }
    }
    return 0;
}

Cfile::~Cfile() {
    if (error == 0) {
        if (filePtr)
        core.fileSystem.setFilePtr(filePtr);
        if (dirPtr)
        core.fileSystem.setDirPtr(dirPtr); 
        core.fileSystem.closeFile();
    }
}
int Cfile::getSize() {
    if (error == 0)
        return info->fileSize;
    return 0;
}

int Cfile::setCursor(int command, int apos, u32 result) {
    if (error == 0) {
        return core.fileSystem.commandFile(command, "", apos, result);
    }
}

int Cfile::getError() {
    return error;
}

int Cfile::command(int acommand, int offset, u32 result) {
    core.fileSystem.setFilePtr(filePtr);
    if (error == 0) {
        switch (acommand) {
            case SEEK_SET:
            {
                return setCursor(SEEK_SET_FILE,offset,result);
            }
            case SEEK_CUR:
            {
                return setCursor(SEEK_CUR_FILE,offset,result);
            }
            case SEEK_END:
            {
                return setCursor(SEEK_END_FILE,offset,result);
            }
            case GET_NEXT_FILE:
            {
                
                if (dirPtr) core.fileSystem.setDirPtr(dirPtr);
                else return 0;
                int error;
                if (error = core.fileSystem.commandFile(GET_NEXT_FILE, "", 0, 0)) { //si no hay error y no se acabado los archivos  
                    dirPtr = core.fileSystem.getDirPtr();
                    info = core.fileSystem.getFileInfo();
                    return 1;
                }

                return 0;
            }
            default: break;
        }
    }
    return error;
}

void Cfile::setIdFile(int aidFile) {
    idFile = aidFile;
}

int Cfile::getIdFile() {
    return idFile;
}

int Cfile::getNextFile() {
    core.fileSystem.setFilePtr(filePtr);
    return core.fileSystem.command(GET_NEXT_FILE, "");
}

char *Cfile::getName() {
    return info->name;
}

int Cfile::getTypeFile() {
    return info->typeFile;
}
        