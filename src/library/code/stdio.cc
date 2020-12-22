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

enum commandConsole {
    cmcColor = 1, cmcGotoXY = 2, cmcWhereX = 3, cmcWhereY = 4,
    cmcPrintf = 5, cmcLock = 6, cmcUnlock = 7, cmcGetChar = 8,
    cmcPutChar = 9, cmcReadChar = 10, cmcWrite = 11
};


#include "../includes/stdio.h"
#include "../includes/conio.h"
#include "../includes/stdlib.h"
#include "../includes/syscall.h"
#include "../includes/conversion.h"
#include "../includes/string.h"

extern FILE *stdout;
extern FILE *stdin;

//Definiciones del Kernel
#define FILE_R                0x1    //Flag Lectura de Archivo
#define FILE_W                0x2    //Flag Escritura de Archivo
#define FILE_RW               0x3    //Lectura y Escritura de Archivo
#define FILE_TRUNCATE         0x4    //Sobrescribe un archivo
#define FILE_CREATE           0x8    //Crea un archivo

enum commandFileSystem {
    cmfCreateDir = 1, cmfDeleteFile = 2, cmfDeleteFolder = 3, cmfRenameFile = 4, cmfMoveFile = 5,
    cmfOpen = 6, cmfRead = 7, cmfWrite = 8, cmfClose = 9, cmfGetEntDir = 10, cmfLSeek = 11
};

//ok
int open(const char* filename, int flags, ...) {
    return sysCall_3(sysFileSystem, cmfOpen, (int) filename, flags);
}
//ok
int read(int fd, void* abuffer, int count) {
    return sysCall_4(sysFileSystem, cmfRead, fd, (int) abuffer, count);
}
//ok
int write(int fd, const void* abuffer, int count) {
    return sysCall_4(sysFileSystem, cmfWrite, fd, (int) abuffer, count);
}
//ok
int close(int fd) {
    return sysCall_2(sysFileSystem, cmfClose, fd);
}
//ok    fseek(fileFont, 256, SEEK_SET);
int lseek(int fd, int aoffset, int awhence) {
    int result;
    int error;
    error = sysCall_5(sysFileSystem, cmfLSeek, fd, aoffset, awhence, (int) &result);
    return result;
}
//ok
int flagsresult(const char* mode, int *kflag) {
    int flags = 0x0; //banderas para stdio
    *kflag = 0x0;    //banderas para el kernel
    while (1) {
        switch (*(mode++)) {
            case 0x0: return flags;
            case 'r': { flags |= FILE_CAN_READ; *kflag |= FILE_R; break;}
            case 'w': { flags |= FILE_CAN_WRITE; *kflag |= (FILE_W | FILE_TRUNCATE | FILE_CREATE); break; }
            case '+': { flags |= (FILE_CAN_WRITE | FILE_CAN_READ);*kflag |= FILE_W; break; }
            case 'a': { flags |= FILE_CAN_WRITE; *kflag |= (FILE_W | FILE_CREATE); break; }
            default: break;
        }
    }
}
//ok
FILE* fopen(const char* path, const char* mode) {
    int fd, kflags = 0, flags = 0;
    flags = flagsresult(mode, &kflags);
    fd = open(path, kflags);

    if (fd == 0) return 0;

    FILE* stream = (FILE*) malloc(sizeof (FILE));
    if (stream == 0) return 0;
    stream->buffer = new char[SIZE_BUFFER];
    
    if (stream->buffer == 0) {
        delete(stream);
        return 0;
    }

    stream->fd = fd;
    stream->flags = flags;
    stream->posbuffer = 0;
    stream->sizebuffer = SIZE_BUFFER;
    stream->countdatabuffer = 0;
    return stream;
}
//ok
FILE* fdopen(int fd, const char* mode) {
    int kflags = 0;
    FILE* stream = (FILE*) malloc(sizeof (FILE));
    stream->fd = fd;
    stream->flags = flagsresult(mode, &kflags) | FILE_UNBUFFER;
    stream->posbuffer = 0;
    stream->sizebuffer = SIZE_BUFFER;
    stream->countdatabuffer = 0;
    return stream;
}
//ok
int fread(void* abuffer, int size, int n, FILE* stream) {
    if (stream == 0) return 0;                          //El ficheo no se pudo abrir
    if (stream->flags & FILE_CAN_READ == 0) {           //Hay permiso de lectura
        stream->flags |= FILE_ERROR;
        return 0;
    }

    int bsize = size*n;
    if (bsize == 0) return 0; //Hay algo que leer
    
    if (stream->fd == 0) return (int)gets((char*)abuffer);

    if (stream->flags & FILE_EOF) return EOF; //Se ha acabado el fichero

    for (int i = 0; i < bsize; i++) {
        int c = fgetc(stream);
        if (stream->flags & FILE_EOF) return i; //Si se acabo el fichero
        ((char*) abuffer)[i] = c;
    }
    return bsize;
}
//ok
int fgetc(FILE* stream) {
    if (stream == 0) return 0;
    if (stream->flags & FILE_CAN_READ == 0) {           //Hay permiso de lectura
        stream->flags |= FILE_ERROR;
        return 0;
    }
    if (stream->fd == 0) return getchar();             //Entrada estandar
    if (stream->flags & FILE_EOF) return EOF;          //No se ha acabado el fichero
    
    if (stream->posbuffer >= stream->countdatabuffer) {           //El buffer esta vacio
        if (stream->flags & BUFFER_WRITE) fflush(stream);         //Es necesario por si hay que seguir leyendo y cargar nuevamente el buffer
        stream->countdatabuffer = read(stream->fd, stream->buffer, stream->sizebuffer);
        if (stream->countdatabuffer == 0) {
            stream->flags |= FILE_EOF;
            return EOF;
        }
        stream->posbuffer = 0;
    }
    stream->flags |= BUFFER_READ;
    return (int) stream->buffer[stream->posbuffer++];
}
//ok
int fflush(FILE* stream) {
    if (stream == 0) return 0;
    if (stream->flags & BUFFER_READ) { //Si se estaba leyendo anteriormente
        int pos = (int) stream->posbuffer - (int) stream->countdatabuffer; //Obtengo cuantos byte se han leido del buffer
        if (pos != 0) lseek(stream->fd, pos, SEEK_CUR); //Muevo el cursor del fichero
        stream->countdatabuffer = 0; //Reinicio lectura
        stream->posbuffer = 0;
        stream->flags &= ~BUFFER_READ; //Se actualiza el flag de lectura 
    } else if (stream->posbuffer > 0) { //Si hay algo que escribir del buffer
        int error = write(stream->fd, stream->buffer, stream->posbuffer); //Escribo el contenido del buffer
        if (error = 0) {
            stream->flags |= FILE_ERROR;
            return EOF;
        } //Si hubo error retorno con error
        stream->flags &= ~BUFFER_WRITE; //Buffer ya escrito
        stream->posbuffer = 0;
        return error;
    }
    return 0;
}
//ok
int fwrite(const void* abuffer, int size, int n, FILE* stream) {
    if (stream == 0) return 0;
    if (stream->flags & FILE_CAN_WRITE == 0) { //Hay permiso de escritura
        stream->flags |= FILE_ERROR;
        return 0;
    }
    int bsize = size*n;
    if (bsize == 0) return 0;
    
    if (stream->fd == 1) return sysCall_3(sysConsole, cmcWrite, (int) abuffer, bsize);    //Se imprime en la salida estandar

    if (stream->flags & BUFFER_READ) fflush(stream); //Actualizo el estado del buffer en el fichero si se estaba leyendo  
    if (bsize > stream->sizebuffer || stream->flags & FILE_UNBUFFER){    //Si lo que se va escribir es mayor que el buffer o no tiene buffer                                               
       int error = write(stream->fd, abuffer, bsize);        //Escribo el buffer
       if (error = 0) return 0;
       return EOF;
    } else {                                         //Si cabe en el buffer
        char* c = (char*)abuffer;
        for (int i = 0; i < bsize; i++, c++) { //Se escribe en el buffer
            fputc(*c,stream);
        }
    }
    return 0;
}
//ok
int fputc(int c, FILE* stream) {
    if (stream == 0) return 0;
    if (stream->flags & FILE_CAN_WRITE == 0) { //Hay permiso de escritura
        stream->flags |= FILE_ERROR;
        return 0;
    }
    if (stream->fd == 1) return putchar(c);      //Salida estandar
    if (stream->flags & FILE_UNBUFFER) {         //Si no hay buffer
        int error = write(stream->fd, &c, 1);    //Escribo el caracter
        if (error = 0) return 0;
        return EOF;
    }
    
    if (stream->posbuffer >= (stream->sizebuffer - 1)) { //Si esta lleno el buffer    
        int error = fflush(stream); //Actualizo el buffer
        if (error == 0) {
            stream->flags |= FILE_ERROR;
            return 0;
        }
    }
    stream->buffer[stream->posbuffer++] = char(c);
    stream->flags |= BUFFER_WRITE;
    return 0;
}
//ok
int fclose(FILE* stream) {
    if (stream == 0) return 0;
    fflush(stream);
    return close(stream->fd);
}
//ok
int feof(FILE* stream) {
    if (stream == 0) return 0;
    return (stream->flags & FILE_EOF);
}
//ok
int ferror(FILE *stream) {
    if (stream == 0) return 0;
    return (stream->flags & FILE_ERROR);
}
//ok
int fgetpos(FILE* stream, int *apos) {
    if (stream == 0) return 0;
    *apos = ftell(stream);
    return 0;
}
//ok
int fsetpos(FILE* stream, const int *apos) {
    if (stream == 0) return 0;
    return lseek(stream->fd, *apos, SEEK_SET);
}
//ok
void clearerr(FILE* stream) {
    if (stream == 0) return;
    stream->flags &= ~(FILE_EOF | FILE_ERROR);
}
//ok
int fseek(FILE* stream, int aoffset, int awhence) {
    if (stream == 0) return 0;
    fflush(stream);                         //Se salva el buffer para cambiar posicion
    lseek(stream->fd, aoffset, awhence);
    stream->countdatabuffer = 0;
    stream->posbuffer = 0;
    stream->flags = 0;
    return 0; //(stream->cursor != 0 ? 0 : -1);
}
//ok
void rewind(FILE* stream) {
    if (stream == 0) return;
    clearerr(stream);
    fseek(stream, 0, SEEK_SET); //0L constante larga
}
//ok
int ftell(FILE* stream) {
    if (stream == 0) return 0;
    if (stream->flags & (FILE_ERROR | FILE_EOF)) return EOF;
    int pos = lseek(stream->fd, 0, SEEK_CUR);   //Ve a la posicion relativa + 0
    if (stream->flags & BUFFER_READ) pos -= (stream->countdatabuffer - stream->posbuffer); //Se actualiza con lo leido
    else if (stream->flags & BUFFER_WRITE) pos += stream->posbuffer;                  //Se actualiza con lo escrito
    return pos;
}
//ok
int remove(const char *path) {
    return sysCall_2(sysFileSystem, cmfDeleteFile, (int) path);
}
//ok
int rename(const char *name, const char *newname) {
    return sysCall_3(sysFileSystem, cmfMoveFile, (int) name, (int) newname);
}
//ok
int sup_fprintf(FILE *stream, const char*string, int *ebp) {
    if (string == 0x0) return 0;
    const char *ptrCar = string;
    char car = *(ptrCar++);
    char buffer[14];
    while (car != 0x0) {
        switch (car) {
            case '\\':
            { //codigo para ejecutar salto de linea           
                car = *(ptrCar++); //proximo caracterptrCar++;
                if (car == 'n' || car == 'N') //si es /n o /N 
                {
                    fputc(CAR_JMPLINE, stream);
                    break;
                } //se envia salto de linea
                fputc('/', stream);
                break; //si no se imprime /
            };
            case '%':
            {
                car = *(ptrCar++); //proximo caracterptrCar++;
                if (car == 'i' || car == 's' || car == 'c' || car == 'h') {
                    if (car == 'i') //si es un entero %i
                    {
                        IntToStrChar(*ebp, buffer); //lo  convierto a string
                        sup_fprintf(stream, buffer, ebp); //lo muestro
                    } else if (car == 'h') {
                        IntToHexChar(*ebp, buffer, 8); //lo  convierto a string
                        sup_fprintf(stream, buffer, ebp); //lo muestro                       
                    }
                    else if (car == 's') //si es un string %s
                        sup_fprintf(stream, (char*) *ebp, ebp);
                    else if (car == 'c') //si es un char
                        fputc((char) *ebp, stream);

                    ebp++; //preparo puntero a proxima varible
                    break;
                }
                fputc('%', stream);
                break; //si no se imprime /
            };
            default:
            {
                fputc(car, stream);
                break;
            } //si es un car se imprime  
        };
        car = *(ptrCar++);
    }
}

int fprintf(FILE *stream, const char*string, ...) {
    if (stream == 0) return 0;  
    int *ebp;
    asm("movl %%ebp,%0" : "=m"(ebp));
    ebp += 4;
    return sup_fprintf(stream, string, ebp);
}
//ok
int printf(const char *string, ...) {
    int *ebp;
    asm("movl %%ebp,%0" : "=m"(ebp));
    ebp += 3;
    return sup_fprintf(stdout, string, ebp);
}
//ok
int putchar(const int c){
   return sysCall_2(sysConsole, cmcPutChar, c);
}
//ok
int getchar(void) {
    int c = 0x0, n = 0;
    c = sysCall_1(sysConsole, cmcReadChar);
    if (c == 0x0) {
        do {
            c = sysCall_1(sysConsole, cmcGetChar);
            n++;
        } while (c != CAR_JMPLINE || n == 0);
        c = sysCall_1(sysConsole, cmcReadChar);
    }
    return c;
}
//ok
int puts(const char *string) {
    int count = strlen(string);
    if (count) return sysCall_3(sysConsole, cmcWrite, (int) string, count);
    return 0;
}
//ok
int fputs(const char *string, FILE* stream) {
    if (stream == 0) return 0;
    if (stream->fd == 1) return puts(string); //Salida estandar
    while (*string) {
        int error = fputc(*string++, stream);
        if (error == EOF) {
            stream->flags |= FILE_ERROR;
            return EOF;
        }
    }
    fflush(stream);
    return 0;
}
//ok
char* fgets(char *string, int n, FILE* stream) {
    if (stream == 0) return 0;
    if (stream->fd == 0) return gets(string); //Entrada estandar
    if (stream->flags & FILE_CAN_READ == 0) { //Hay permiso de lectura
        stream->flags |= FILE_ERROR;
        return 0;
    }
    int i;
    for (i = 0; i < n; i++) {
        const int c = fgetc(stream);
        if (c == EOF) break;
        string[i] = c;
        if (c == CAR_JMPLINE) break;
    }
    if (i == 0 || ferror(stream)) return 0;
    string[i + 1] = 0x0;
    return string;
}
//ok
char *gets(char *string) {
    char car;
    int n = 0;
    do {
        car = getchar();
        if (car != CAR_JMPLINE) string[n++] = car;
    } while (car != CAR_JMPLINE || n == 0x0);
    string[n] = 0x0;
    return string;
}
//ok
int sup_getstring(char *string, FILE *stream) {
    char c;
    int n = 0;
    do {
        c = fgetc(stream);
        if (c == EOF) return EOF;
        if (c != ' ' && c != CAR_JMPLINE && c!= CAR_RETCURSOR) {     
            string[n] = c;
            n++;
        }
    } while ((c != ' ' && c != CAR_JMPLINE && c!= CAR_RETCURSOR) || n == 0);
    string[n] = 0x0;
}
//ok
int sup_fscanf(FILE *stream, const char*string, int *ebp) {
    if (string == 0x0) return 0;
    const char *ptrCar = string;
    char *argChar;
    int sChar;
    char car = *(ptrCar++);
    int *sInt;
    char buffer[14];
    while (car != 0x0) {
        switch (car) {
            case '%':
            {
                car = *(ptrCar++); //proximo caracter ptrCar++;
                if (car == 'c') //si es un char
                {
                    if (stream->fd == 0) sChar = getchar();
                    else {
                        sChar = fgetc(stream);
                        if (sChar == EOF) return EOF;
                    }
                    argChar = (char*) *ebp;
                    *argChar = sChar;
                    ebp++;
                } else if (car == 's') //si es una cadena
                {
                    argChar = (char*) *ebp;
                    if (stream->fd == 0) gets(argChar);
                    else {
                        sChar = sup_getstring(argChar, stream);
                        if (sChar == EOF) return EOF;
                    }
                    ebp++;
                } else if (car == 'i') //si es una cadena
                {
                    if (stream->fd == 0) gets(buffer);
                    else {
                        sChar = sup_getstring(buffer, stream);
                        if (sChar == EOF) return EOF;
                    }
                    sInt = (int*) *ebp;
                    *sInt = atoi(buffer);
                    ebp++;
                }
                break;
            }
        };
        car = *(ptrCar++);
    }
}
//ok
int fscanf(FILE *stream, const char*string, ...) {
    if (stream == 0) return 0;
    if (stream->flags & FILE_CAN_READ == 0) { //Hay permiso de lectura
        stream->flags |= FILE_ERROR;
        return 0;
    }    
    int *ebp;
    asm("movl %%ebp,%0" : "=m"(ebp));
    ebp += 4;
    return sup_fscanf(stream, string, ebp);
}
//ok
int scanf(char *string, ...) {
    int *ebp;
    asm("movl %%ebp,%0" : "=m"(ebp));
    ebp += 3;
    return sup_fscanf(stdin, string, ebp);
}