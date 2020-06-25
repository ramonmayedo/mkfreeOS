#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H
#include "../../uses/list.h"
#include "../../defines/types.h"

struct SsharedMemory{
    u8 *virtualMemory;
    u8 *phisicalAdress;
    int idProcessFather;
    int password;
    int flags;
    int id;
};

enum commandSharedMemory{
    cmsGetZone = 1, cmsAddZone = 2, cmsSharedZone = 3
};

class CsharedMemory {
public:
    CsharedMemory();
    void initialized();
    int addSharedMemory(int idProcess, int flags, int password, u8 *virtualMemory, u8 *phisicalAdress);
    int deleteSharedMemory(int idProcess, int idSharedMemory);
    int setSharedMemory(int idProcess, int idSharedMemory);
    int deleteSharedMemory(int idProcess);
    int command(int acommand, int parameter1, int parameter2, int parameter3);
private:
    Clist *sharedMemory;
    int idCount;
};

#endif

