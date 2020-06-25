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

#include "../includes/dirent.h"
#include "../includes/syscall.h"
#include "../includes/stdlib.h"

#define DIR_R 0x10

#define ATT_DIR  0x01
#define ATT_ARCH 0x02
#define ATT_VOL  0x04

enum commandFileSystem {
    cmfCreateDir = 1, cmfDeleteFile = 2, cmfDeleteFolder = 3, cmfRenameFile = 4, cmfMoveFile = 5,
    cmfOpen = 6, cmfRead = 7, cmfWrite = 8, cmfClose = 9, cmfGetEntDir = 10
};

DIR *opendir(char *dirname) {
    int flags = DIR_R, fd;
    fd = sysCall_3(sysFileSystem, cmfOpen, (int) dirname, flags);
    if (fd == 0) return 0;
    DIR* dir = (DIR*) malloc(sizeof (DIR));
    dir->fd = fd;
    dir->flags = flags;
    return dir;
}

dirent *readdir(DIR *dirp) {
    dirent * ent = new dirent;
    int endDir = sysCall_3(sysFileSystem, cmfGetEntDir, dirp->fd, (int) ent);
    if (endDir == 0) return 0;
    return ent;
}

int closedir(DIR *dirp) {
    if (dirp) {
        int status = sysCall_2(sysFileSystem, cmfClose, dirp->fd);
        delete(dirp);
        return status;
    }
    return 0;
}

int createdir(const char *name) {
    return sysCall_2(sysFileSystem, cmfCreateDir, (int) name);
}

int deleteDir(const char *name) {
    return sysCall_2(sysFileSystem, cmfDeleteFolder, (int) name);
}