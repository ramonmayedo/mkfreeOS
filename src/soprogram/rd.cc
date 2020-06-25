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

#include <cprogram.h>
#include <stdio.h>
#include <conio.h>
#include <dirent.h>
#include <string.h>

char strToCat[255];

int kmain(int argc, char** argv) {
    DIR *dir;
    dirent *archivo;
    bool existe = false;
    if (argc > 1) {
        int leng = strlen(argv[1]);
        strToCat[0] = 0x0;
        int size = strlen(argv[2]);
        if (leng) {
            strcat(strToCat, argv[1]);
            if (size) {
                if (strToCat[leng - 1] != '/') {
                    strToCat[leng++] = '/';
                    strToCat[leng] = 0x0;
                }
            }
        }
        if (size) 
        strcat(strToCat, argv[2]);
        
        dir = opendir(strToCat);
        if (dir) existe = true;
        else {

            dir = opendir(argv[2]);
            if (dir) existe = true;
        }

        if (existe == true) {
            while ((archivo = readdir(dir))) {
                printf("%s", archivo->name/*, (archivo->typeFile == ATT_DIR ? " <DIR> " : "")*/);
               // if (archivo->typeFile == ATT_ARCH) printf(" %i bytes \n", archivo->fileSize);
               /* else*/ printf("\n");
            }
             closedir(dir);
        } else
            printf(" El directorio \"%s\" no existe \n", strToCat);
    }

    unblockDisplay();
    return 0;
}

