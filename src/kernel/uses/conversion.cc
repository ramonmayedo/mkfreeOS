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

#include "conversion.h"
#include "../defines/types.h"

Cconversion::Cconversion() {
}

void Cconversion::IntToStrChar(int num, char* string) {
    intToChar(num, string, 10, 0);
}

void Cconversion::IntToHexChar(int num, char *string, int acomplete) {
    intToChar(num, string, 16, acomplete);
}

void Cconversion::intToChar(int num, char *string, int asistema, int acomplete) {
    short index = 0, sign = 0;
    char charTable[17] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    u32 numAux = num, resto = 0;

    char buffer[14]; //numero de digitos

    if ((int) numAux < 0) {
        if (asistema != 16) {
            sign = 1;
            string[0] = '-';
            numAux *= -1;
        }
    }

    while (numAux >= asistema) {
        resto = numAux % asistema;
        numAux = numAux / asistema;
        buffer[index] = charTable[resto];
        index++;
    }

    if (numAux != 0 || index == 0)
        buffer[index] = charTable[numAux];
    else index--;

    while (index + 1 < acomplete) {
        index++;
        buffer[index] = charTable[0];
    }

    for (int i = index, j = sign; i >= 0; i--, j++)
        string[j] = buffer[i];

    string[index + 1 + sign] = 0x0; //fin del string

    return;
}