#ifndef STDIO_H
#define STDIO_H
//Banderas para los archivos
#define FILE_CAN_READ   0x01
#define FILE_CAN_WRITE  0x02
#define FILE_CAN_RW     0x03
#define FILE_ERROR      0x04
#define FILE_EOF        0x08
#define FILE_UNBUFFER   0x10
#define BUFFER_READ     0x20
#define BUFFER_WRITE    0x40

//Definiciones
#define EOF             -1
#define SIZE_BUFFER     2048

//Para fseek
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef struct FILE {
    int fd;
    int flags;
    char *buffer;
    unsigned int posbuffer;
    unsigned int sizebuffer;
    unsigned int countdatabuffer;
} FILE;



FILE* fopen(const char* path, const char* mode);
FILE* fdopen(int fd, const char* mode);
int fread(void* abuffer, int size, int n, FILE* stream);
int fwrite(const void* abuffer, int size, int n, FILE* stream);
int fclose(FILE* stream);
int feof(FILE* stream);
int fgetpos(FILE* stream, int *apos);
int fsetpos(FILE* stream, const int *apos);
int fseek(FILE* stream, int aoffset, int awhence);
void rewind(FILE* stream);
int ftell(FILE* stream);
int fgetc(FILE* stream);
char* fgets(char *string,int n,FILE* stream);
char *gets(char *string);
int fputc(int c, FILE* stream);
int fputs(const char *string, FILE* stream);
int remove(const char *path);
int rename(const char *name, const char *newname);
void clearerr(FILE* stream);
int fprintf(FILE *stream, const char*string, ...);
int printf(const char *string, ...);
int putchar(const int c);
int getchar(void);
int puts(const char *string);
int fscanf(FILE *stream, const char*string, ...);
int ferror(FILE *stream);
int fscanf(FILE *stream, const char*string, ...);
int scanf(char *string, ...);
int fflush(FILE* stream);


#endif

