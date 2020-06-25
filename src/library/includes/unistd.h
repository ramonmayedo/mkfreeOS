#ifndef UNISTD_H
#define UNISTD_H

void* sbrk(int incSize);
void exit(int astatus);
int execv(const char* fileName, char* const argv[]);
int getCommand(int *command, int *pidTrasmisor, int *parameter1, int *parameter2);
int sendCommand(int command, int pidReceptor, int parameter1, int parameter2);
int waitCommand(int command, int *pidTrasmisor, int *parameter1, int *parameter2);
int writeBufferIPC(int pid,char *buffer,int offset , int size);
int readBufferIPC(int pid,char *buffer,int offset ,int size);
void diskflush();
int newSemaphore();
int delSemaphore(int sid);
int blockSemaphore(int sid);
int unblockSemaphore(int sid);
int getPid();
int shmem(int size);
int setshmem(int pid, int sid, int size);
#endif

