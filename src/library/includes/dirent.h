#ifndef DIRENT_H
#define DIRENT_H

//Para todos los sistemas de archivos
#define ATTRK_DIRECTORY  0x01
#define ATTRK_ARCHIVE    0x02
#define ATTRK_VOLUME_ID  0x04

typedef struct {
    int fd;
    int flags;
} DIR;

typedef struct dirent {
    char name[255];
    int typeFile;
    unsigned int fileSize;
    unsigned int handle;
};


DIR *opendir(char *dirname);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);
int createdir(const char *name);
int deleteDir(const char *name);

#endif

