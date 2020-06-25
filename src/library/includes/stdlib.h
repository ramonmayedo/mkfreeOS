#ifndef STDLIB_H
#define STDLIB_H

#define u32 unsigned int
#define u8 unsigned char

//Gestión de memoria dinámica
void* malloc(u32 size );
void free(void *memory);
void* realloc(void* memory, u32 size);
void* calloc(u32 size1, u32 size2);
//Conversión de tipos
int atoi(char *str);
long int strtol(char *str);

int reset();
int shutDown();

#endif

