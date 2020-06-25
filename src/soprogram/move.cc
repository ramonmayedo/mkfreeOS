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

#include <program.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <dirent.h>
char strToCat[255];

int kmain(int argc, char** argv) {
    DIR *dir;
    dirent *archivo;
    bool existe = false;
    if (argc > 3) {
        int leng = strlen(argv[1]);
        strToCat[0] = 0x0;
        if (leng) {
            strcat(strToCat, argv[1]);
            int size = strlen(strToCat);
            if (size) {
                if (strToCat[size - 1] != '/') {
                    strToCat[size++] = '/';
                    strToCat[size] = 0x0;
                }
            }
        }
        strcat(strToCat, argv[2]);

        if (strlen(argv[3]) == 0) {
            printf("Escriba un nombre para renombrar\n");
        }

        if (rename(strToCat, argv[3]) == 0) {
            printf("Movido : %s \n", strToCat);
            printf("a : %s \n", argv[3]);
        } else if (rename(argv[2],argv[3]) == 0) {
            printf("Movido : %s \n", argv[2]);
            printf("a : %s \n", argv[3]);
        } else printf("Error no se pudo mover\n");
    }  
    unblockDisplay();
    return 0;
}

