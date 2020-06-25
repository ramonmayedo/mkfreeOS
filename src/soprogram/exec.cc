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
#include <string.h>
#include <application.h>

char strToCat[255];

int kmain(int argc, char** argv) {
    bool existe = false;
    int  isExec;
    Capplication *program;
    if (argc > 1) {
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
        program = new Capplication("elf", strToCat, 10);
        for (int i = 3; i < argc; i++)
            program->addArg(argv[i]);

        isExec = program->execute();
    } else
        unblockDisplay();

    if (!isExec) {
        printf("Ejecutable \"%s\" no valido\n", strToCat);
        unblockDisplay();
    }
    return 0;
}

