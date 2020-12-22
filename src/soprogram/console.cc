/*Copyright (C) 2019  Ram�n Mayedo Morales (ramonmayedo@gmail.com)

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

#include <cprogram.h>
#include <application.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <dirent.h>

int scanLine(char *line);
void dirExist(char *path);

char dirPath[255];
char strToCat[255];

int kmain(int argc, char *argv[]) {
    char line[255];
    
    dirPath[0] = 0x0;
    while (1) {
        memset(line, 0x0, 254);
        blockDisplay();
        printf("%s>> ", dirPath);
        unblockDisplay();
        gets(line);
        scanLine(line);
    }
    while (1);
    return 0;
}

int scanLine(char *line) {
    char *tok, *command, *arg;
    char sep[] = " ";
    int lenght;
    Capplication *console;
    tok = strtok(line, sep);
    int indexTok =0;
    if (tok != 0x0) {
        bool exec = false;
        lenght = strlen(tok);
        command = new char[lenght];
        strcpy(command, tok);

        if (strcmp("help", command) == 0) {
            exec = true;
            console = new Capplication("help", "a/kernel/program/help.elf", 10);
        } else if (strcmp("reset", command) == 0) {
            console = new Capplication("reset", "a/kernel/program/reset.elf", 10);
            exec = true;
        } else if (strcmp("ldir", command) == 0) {
            console = new Capplication("rd", "a/kernel/program/rd.elf", 10);
            char *dir = new char;
            dir = dirPath;
            console->addArg(dir);
            exec = true;
        } else if (strcmp("cdir", command) == 0) {
            tok = strtok(line, sep);
            dirExist(tok);
            tok = strtok(0, sep);
            delete(command);
            return 0;
        } else if (strcmp("time", command) == 0) {
            console = new Capplication("time", "a/kernel/program/time.elf", 10);
            exec = true;
        } else if (strcmp("exec", command) == 0) {
            console = new Capplication("exec", "a/kernel/program/exec.elf", 10);
            char *dir = new char;
            dir = dirPath;
            console->addArg(dir);
            exec = true;
        } else if (strcmp("mdir", command) == 0) {
            console = new Capplication("mdir", "a/kernel/program/mdir.elf", 10);
            char *dir = new char;
            dir = dirPath;
            console->addArg(dir);
            exec = true;
        } else if (strcmp("rename", command) == 0) {
            console = new Capplication("rename", "a/kernel/program/rename.elf", 10);
            char *dir = new char;
            dir = dirPath;
            console->addArg(dir);
            exec = true;
        } else if (strcmp("delete", command) == 0) {
            console = new Capplication("delete", "a/kernel/program/delete.elf", 10);
            char *dir = new char;
            dir = dirPath;
            console->addArg(dir);
            exec = true;
        } else if (strcmp("deldir", command) == 0) {
            console = new Capplication("deldir", "a/kernel/program/deldir.elf", 10);
            char *dir = new char;
            dir = dirPath;
            console->addArg(dir);
            exec = true;
        } else if (strcmp("move", command) == 0) {
            console = new Capplication("move", "a/kernel/program/move.elf", 10);
            char *dir = new char;
            dir = dirPath;
            console->addArg(dir);
            exec = true;
        }

        tok = strtok(line, sep);

        if (exec == true) {
            while (tok != 0x0) {
                lenght = strlen(tok);
                arg = new char[lenght];
                strcpy(arg, tok);
                console->addArg(arg);
                tok = strtok(line, sep);
            }
            blockDisplay();
            if (console->execute() == 0){
                printf("No se pudo ejecutar el proceso\n");
                unblockDisplay();
            }
            console->deleteArg();
        } else {
            tok = strtok(0, sep); //se inicializa la funcion
            printf("No se reconoce este comando \"%s\" \n", command);
            unblockDisplay();
            delete(command);
        }
    }

    return 0;
}

void dirExist(char *path) {
    DIR *dir;
    strToCat[0] = 0x0;
    strcat(strToCat, dirPath);
    int size = strlen(strToCat);
    if (size) {
        if (strToCat[size - 1] != '/') {
            strToCat[size++] = '/';
            strToCat[size] = 0x0;
        }
    }
    strcat(strToCat, path);
    dir = opendir(strToCat);

    if (dir) {
        dirPath[0] = 0x0;
        strcpy(dirPath, strToCat);
    } else {
        dir = opendir(path);
        if (dir) {
            strcpy(dirPath, path);
            closedir(dir);
        } else
            printf(" No existe el directorio \"%s\" \n", strToCat);
    }
}