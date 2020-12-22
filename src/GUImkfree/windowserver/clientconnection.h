#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H
#include "../basics/socketipc.h"
#include "../basics/list.h"
#include "window.h"

struct Sconnection {
    SsocketIPC *socketIPC;
    Swindow *window;
    int cid;
};

class SclientConnection {
public:
    SclientConnection(int apidServer);
    int addConnection(int pidClient);
    ~SclientConnection();
private:
    int pidServer;
    Clist connections;
    int cid;
};

#endif

