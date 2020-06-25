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

#ifndef SERVER_H
#define SERVER_H
#include "../basics/control.h"
#include "../basics/list.h"
#define ERR_CONNECTION_EXIST 1
#define ERR_CONNECTION_FULL  2

#define ERR_COMMAND_INAVLID  100

#define MAX_CLIENT_CONNECTIONS 20

enum eCommandServer{
    cmsCreateConnection = 1, cmsCreateWindows = 2, cmsPaintWindows = 3,
    cmsActiveWindows = 4, cmsMinimizedWindows = 5, cmsRepaintZone = 6,
    cmsRestoreWindows = 7, cmsSetPIDServer = 8 ,cmsCloseWindows = 13, cmsSetSharedMemory = 14
};

enum eEventType {
    cmeClickDown = 8, cmeClickUp = 9, cmeKeyDown = 10, cmeMauseMove = 11, cmeDoubleClick = 12
};

struct Swindows {
    int state;
    int index;
    Grect *property;
};

struct Sconnection {
    int cid;
    int pid;
    Swindows windows;
};

class Gserver {
public:
    Gserver();
    int activate();
     int executeCommnd(int command, int pidClient, int parameter1, int parameter2);
    int addConnection(int pidClient);
    Sconnection* existConnection(int pidClient);
    int createWindows(Grect *windows, int pid);
    int eventOcurred(eEventType event, Grect &position, int value);
    //int getWindowsActived(Grect *windows);
    int paintComponent(Gcontrol *control);
    int getPaintArea(Gcontrol *control);
    Sconnection *getActiveWindows(Grect position);
private:
    Clist *actives,*minimized;
    Sconnection *activeWindows;
    int countCID;
    int screeSize,idPage;
protected:
    Sconnection *getNextConnection();
    void repaintIntersectWindows();
    Sconnection *getConnection(int pid);
};


#endif

