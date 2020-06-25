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

#include "server.h"
#include "../../library/includes/cprogram.h"
#include "../../library/includes/unistd.h"
#include "../components/Gcursor.h"
#include "../basics/elapseTime.h"
#include "../components/Gwindows.h"
#include "../../library/includes/thread.h"

void* getMouseEvent(void*);
void* getKeyEvent(void*);
 
 Gserver *server;
 int pidMainBar,pidServer, pidFileExplorer;
int kmain(int , char **) {
    pidServer = getPid();
    server = new Gserver();
    char **arg = 0;
    changeModeAdvance(1,800,600);
    pidMainBar = execv("a/destokp.elf", arg);
    sendCommand(cmsSetPIDServer, pidMainBar, pidServer, 0);
    pidFileExplorer = execv("a/fileExplorer.elf", arg);
    sendCommand(cmsSetPIDServer, pidFileExplorer, pidServer, 0);
    cthread(getMouseEvent);
    cthread(getKeyEvent);
    server->activate();
    while (1);
    return 0;
}

void* getMouseEvent(void*) {
    int dx = 0, dy = 0, dz = 0;
    int x = 0, y = 0;
    Gcursor *cursor = new Gcursor(0, 0, 16, 16);
    Gcursor *backCursor = new Gcursor(0, 0, 16, 16);
    server->getPaintArea(backCursor);
    server->paintComponent(cursor);
    GelapseTime timeClick;
    bool clicked = false;
    do {
        int button = getMouseState(&dx, &dy, &dz);       
            x += dx;
            y += dy;
            if (x > 1024) x = 1024;
            else if (x < 0) x = 0;
            if (y > 728) y = 728;
            else if (y < 0) y = 20;
            server->paintComponent(backCursor);
            cursor->left = backCursor->left = x;
            cursor->top = backCursor->top = 728 - y;
            server->getPaintArea(backCursor);
            server->paintComponent(cursor);
            Grect point(cursor->left, cursor->top, 2, 2);

        if (clicked == true && button == 0) { //Mouse Up
            server->eventOcurred(cmeClickUp, point, 0);
            clicked = false;
        } else if (button != 0 && clicked == false) {//Mouse Down
            if (!timeClick.getStartElpase()) { //DoubleClick
                server->eventOcurred(cmeClickDown, point, 0);
                timeClick.start();
            } else if (timeClick.getElapseTimeSecond() < 250) {
                timeClick.stop();
                server->eventOcurred(cmeDoubleClick, point, 0);
            } else {
                timeClick.stop();
                server->eventOcurred(cmeClickDown, point, 0);
            }
            clicked = true;
        } else {
            server->eventOcurred(cmeMauseMove, point, 0);
        }
    } while (1);
}

void* getKeyEvent(void*) {
    do {
        char key = getKeyPress();
        Grect rect(0,0,0,0);
        server->eventOcurred(cmeKeyDown,rect, key);
    } while (1);
}

Gserver::Gserver() {
    countCID = 1;
    actives = new Clist();
    minimized = new Clist();
    screeSize = 800*600*2;
    idPage = shmem(screeSize); //Memoria Compartida para intercambio
}

int Gserver::activate() {
    int command, pidTrasmisor, parameter1, parameter2, recive;
    do {
        recive = getCommand(&command, &pidTrasmisor, &parameter1, &parameter2);
        if (recive) {
            executeCommnd(command, pidTrasmisor, parameter1, parameter2);
        }
    } while (1);
}

int Gserver::executeCommnd(int command, int pidClient, int parameter1, int parameter2) {
    switch (command) {
        case cmsCreateConnection:
        {
            sendCommand(cmsCreateConnection, pidClient, addConnection(pidClient), 0);
            return 0;
        }
        case cmsCreateWindows:
        {
            Grect *windows = new Grect();
            if (parameter1 == 1) //Se lee del buffer ipc
                readBufferIPC(pidClient, (char*) windows, 0, sizeof (Grect));
            int error = createWindows(windows, pidClient);
            sendCommand(cmsCreateConnection, pidClient, error, 0);
            if (!error) {
                activeWindows->windows.index = parameter2;
                sendCommand(cmsSetSharedMemory, pidClient,idPage ,screeSize);
                sendCommand(cmsPaintWindows, pidClient, 0, 0);
                sendCommand(cmsCreateWindows, pidMainBar, pidClient, 0);

            }
            return 0;
        }
        case cmsMinimizedWindows:
        {
            activeWindows = getConnection(parameter1);
            if (activeWindows) {
                actives->remove(activeWindows);
                minimized->add(activeWindows);
                sendCommand(cmsMinimizedWindows, pidMainBar, activeWindows->pid, 0);
                repaintIntersectWindows();
                activeWindows = getNextConnection();
                if (activeWindows)
                    sendCommand(cmsActiveWindows, pidMainBar, activeWindows->pid, 0);
            }
            return 0;
        }
        case cmsRestoreWindows:
        {
            activeWindows = getConnection(parameter1);
            if (activeWindows) {
                minimized->remove(activeWindows);
                actives->add(activeWindows);
                sendCommand(cmsActiveWindows, activeWindows->pid, 0, 0);
                sendCommand(cmsActiveWindows, pidMainBar, activeWindows->pid, 0);
            }
            return 0;
        }
        case cmsCloseWindows:
        {
            Sconnection *closeConnection = getConnection(parameter1);
            if (closeConnection) {
                actives->remove(closeConnection);
                sendCommand(cmsCloseWindows, closeConnection->pid, 0, 0);
                sendCommand(cmsCloseWindows, pidMainBar, closeConnection->pid, 0);
                repaintIntersectWindows();
                if (activeWindows == closeConnection) {
                    activeWindows = getNextConnection();
                    if (activeWindows)
                        sendCommand(cmsActiveWindows, pidMainBar, activeWindows->pid, 0);
                }
                delete(closeConnection);
            }
            return 0;
        }
        default:sendCommand(command, pidClient, ERR_COMMAND_INAVLID, 0);
    }
}

int Gserver::addConnection(int pidClient) {
    Sconnection *newConnection = new Sconnection;
    newConnection->cid = countCID;
    newConnection->pid = pidClient;
    actives->add(newConnection);
    return 0;
}

Sconnection* Gserver::existConnection(int pidClient) {
    Sconnection *connection;
    for (int i = 0; i < actives->count(); i++) {
        connection = (Sconnection*) actives->getItem(i);
        if (connection->pid == pidClient) return connection;
    }
    return 0;
}

int Gserver::createWindows(Grect* windows, int pid) {
    Sconnection *connection = existConnection(pid);
    if (connection == 0) return 1;
    connection->windows.property = windows;
    connection->windows.state = wsNormal;
    activeWindows = connection;
    return 0;
}

Sconnection* Gserver::getActiveWindows(Grect position) {
    Sconnection *connection;
    for (int i = actives->count()-1; i >=0 ; i--) {
        connection = (Sconnection*) actives->getItem(i);
        if (connection->windows.property->intersects(position)) return connection;
    }
    return 0;
}


int Gserver::eventOcurred(eEventType event, Grect &position, int value) {
    if (activeWindows) {
        if (event == cmeClickDown) {
            if (activeWindows->windows.property->intersects(position))
                if (activeWindows->windows.index > 0)
                    return sendCommand(event, activeWindows->pid, position.x(), position.y());
            Sconnection *connection = getActiveWindows(position);
            if (connection) {
                if (activeWindows->windows.index > 0) {
                    sendCommand(cmsActiveWindows, connection->pid, 0, 0);
                    sendCommand(cmsActiveWindows, pidMainBar, connection->pid, 0);
                }
                activeWindows = connection;
                return sendCommand(event, activeWindows->pid, position.x(), position.y());
            }
        return 0;
        } else if (event == cmeClickUp) {
            if (activeWindows->windows.property->intersects(position))
                return sendCommand(event, activeWindows->pid, position.x(), position.y());
        } else if (event == cmeKeyDown) {
            return sendCommand(event, activeWindows->pid, value, 0);
        } else if (event == cmeDoubleClick) {
            return sendCommand(event, activeWindows->pid, position.x(), position.y());
        } else if (event == cmeMauseMove) {
            return sendCommand(event, activeWindows->pid, position.x(), position.y());
        }
    }
    return 0;
}

int Gserver::paintComponent(Gcontrol *control) {
    SvideoArea area;
    if (control->parent == 0) {
        area.left = control->left;
        area.top = control->top;
        area.width = control->width;
        area.height = control->height;
        area.area = control->graphics->getGraphicsBitmap();
        paintArea(&area);
    }
}

int Gserver::getPaintArea(Gcontrol* control) {
    SvideoArea area;
    area.left = control->left;
    area.top = control->top;
    area.width = control->width;
    area.height = control->height;
    area.area = control->graphics->getGraphicsBitmap();
    getArea(&area);
}
 
Sconnection* Gserver::getNextConnection() {
    if (actives->count()){
        return (Sconnection*)actives->getLast();
    }
    return 0;
}

void Gserver::repaintIntersectWindows() {
    Sconnection *connection;
    Grect *intersection = new Grect;
    for (int i = actives->count()-1; i > 0;  i--) {
        connection = (Sconnection*) actives->getItem(i);
        *intersection = *connection->windows.property & *activeWindows->windows.property;
        if (intersection->width()) {
            writeBufferIPC(connection->pid, (char*) intersection, 0, sizeof (Grect)); //Ubicacion en el Buffer IPC
            sendCommand(cmsRepaintZone, connection->pid, 0, 0); //Se solicita pintar zona                  
        }
    }
    delete(intersection);
}

Sconnection* Gserver::getConnection(int pid) {
    Sconnection *connection;
    if (activeWindows->pid == pid) return activeWindows;
    for (int i = 0; i < minimized->count(); i++) {
        connection = (Sconnection*) minimized->getItem(i);
        if (connection->pid == pid) {
            return connection;
        }
    }

    for (int i = 0; i < actives->count(); i++) {
        connection = (Sconnection*) actives->getItem(i);
        if (connection->pid == pid)
            return connection;
    }
    //
    
    return 0;
}
