#ifndef SOCKETIPC_H
#define SOCKETIPC_H
#include "../../library/includes/unistd.h"

class SsocketIPC {
public:
    SsocketIPC(int apid);
    void readData(void *data,int offset, int size);
    void writeData(void *data,int offset, int size);
    void command(int command, int par1, int par2);
    ~SsocketIPC();
private:
    int pid;
};

#endif

