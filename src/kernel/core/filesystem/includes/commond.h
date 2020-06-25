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

#ifndef COMMOND_H

#define COMMOND_H

#define NEW_FOLDER            0x1    //Crear Nuevo Directorio
#define NEW_FILE              0x2    //Crear Nuevo Archivo
#define SET_DIR_WORK          0x3    //Fijar directorio de trabajo a partir del directorio raiz
#define OPEN_DIR              0x4    //Abrir directorio a partir del directorio
#define RENAME                0x5    //Renombrar un archivo
#define GET_NEXT_FILE         0x6    //Obtener el siguiente archivo
#define OPEN_FILE             0x7    //Abre un archivo
#define READ_FILE             0x8    //Lee una archivo
#define WRITE_FILE            0x9    //Escribe un archivo
#define SEEK_CUR_FILE         0xA    //Cambia la posicion del cursor realativa al cursor
#define SEEK_END_FILE         0xB    //
#define SEEK_SET_FILE         0xC    //Incrementa la posicion del archivo al comienzo
#define DELETE_FILE           0xE    //Borrar un archivo
#define DELETE_FOLDER         0xF    //Borrar Carpeta
#define RENAME_FILE           0x10   //Renombrar Archivo
#define MOVE_FILE             0x11   //Mueve de una ruta a otra un archivo


//Para manipulacion desde el Sistema Operativo al Kernel
#define FILE_R                0x1    //Flag Lectura de Archivo
#define FILE_W                0x2    //Flag Escritura de Archivo
#define FILE_RW               0x3    //Lectura y Escritura de Archivo
#define FILE_TRUNCATE         0x4    //Sobrescribe un archivo
#define FILE_CREATE           0x8    //Crea un archivo
#define DIR_R                 0x10   //Lectura de Directorio


#define SEEK_SET              0x0    //Cursor a posicion inicial del archivo
#define SEEK_CUR              0x1    //Fijar a posicion indicada el puntero del archivo
#define SEEK_END              0x2    //Ir a posicion final del archivo

//errores ocurridos dentro de la manipulacion de los archivos
#define NAME_MAX_LENGHT       0x1   //Si el nombre excede 240 caracteres
#define NAME_INVALID          0x2   //Nombre invalido
#define FILE_NO_EXIST         0x3   //El archivo no existe
#define DIRECTORY_ENTRY_FULL  0x4   //No hay entradas vacias en el directorio
#define FILE_EXIST            0x6  //
#define COMMAND_INVALID       0x8   //Comando Invalido



#endif /* COMMOND_H */

