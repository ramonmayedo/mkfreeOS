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

int printHelp(int idHelp);

int kmain(int argc, char *argv[]) {
    if (argc > 1) {
        printf(" help \n");
        if ((strcmp(argv[1], "help") == 0)) {
            printf(" Comando help <aplicacion>   Muestra la ayuda si existe sobre la <aplicacion>\n");
        }
        else printf(" No existe ayuda para este comando \"%s\" \n",argv[1]);
    } else {
        printHelp(0);
    }
    unblockDisplay();
    return 0;
}

int printHelp(int idHelp) {
    switch (idHelp) {
        case 0:
        {
            printf(" Comando                 Descripcion \n");
            printf(" cdir   <ruta>           Cambia la ruta trabajo de la aplicacion\n");
            printf(" delete <ruta>           Borra un archivo\n");
            printf(" deldir <ruta>           Borra un directorio y subdirectorios\n"); 
            printf(" exec   <programa>       Ejecuta un programa\n");
            printf(" ldir   <ruta>           Lista los archivos y directorios \n");
            printf(" mkdir  <nombre>         Crea un Nuevo Directorio \n");
            printf(" move   <org><dest>      Mueve un archivo a otro ruta \n");            
            printf(" process                 Lista todos los procesos en ejecucion \n");
            printf(" rename <ruta><name>     Renombra un archivo o directorio \n");
            printf(" reset                   Reinicia el ordenador \n");
            printf(" time                    Muestra la hora actual\n");
            break;
        }
    }
    return 0;
}