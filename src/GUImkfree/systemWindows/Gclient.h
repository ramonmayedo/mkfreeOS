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

#ifndef GCLIENT_H
#define GCLIENT_H
#include "../basics/rect.h"
#include "../basics/control.h"
#include "../components/Gwindows.h"
#include "../server/server.h"

typedef void (eventOnDraw) (Gwindows *sender);

class Gclient {
public:
    Gclient();
    int createWindows(Gwindows *windows);
    int activate();
    int connected(int pidServer);
    int executeCommnd(int command, int pidServer, int parameter1, int parameter2);
    int applicationCommand(int command);
    int eventOcurred(eEventType event, Grect &position);
    void onClickDown(Gcontrol *sender, int x, int y);
    void onClickUp(Gcontrol *sender, int x, int y);
    void onDoubleClick(Gcontrol *sender, int x, int y);
    void onKeyDown(Gcontrol *sender, int &key);
    void onFocused(Gcontrol *sender);
    void onMouseMove(Gcontrol *sender,int x, int y);
    void leaveMouseMove(Gcontrol *sender);
    Gcontrol *getControl(Gcontrol *control, int x, int y);
    Gcontrol *getAllPosition(Gcontrol *control, int dx, int dy);
    int execApp(char *fileName,char *argv[]);
    eventOnDraw *OnDraw;
private:
    Gwindows *windows;
    int pidServer,pidClient;
    int *bufferShared;
};
#endif

