/*Copyright (C) 2019  Ram�n Mayedo Morales (ramonmayedo@gmail.com)

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

#ifndef GDESTOKP_H
#define GDESTOKP_H
#include "../../basics/rect.h"
#include "../../basics/control.h"
#include "GmainBar.h"
#include "Gscreen.h"

class Gdestokp{
public:
    Gdestokp();
    int createMainMenu(Grect *mainMenu);
    int executeCommnd(int command, int pidServer, int parameter1, int parameter2);
    void onMouseMove(Gcontrol *sender, int x, int y);
    void leaveMouseMove(Gcontrol *sender);
    void onClickDown(Gcontrol *sender, int x, int y);
    void onFocused(Gcontrol *sender);
    void onClickUp(Gcontrol *sender, int x, int y);
    int activate();
    ~Gdestokp();
private:
    int pidClient,pidServer;
    Grect *windows;
    void draw();
    GmainBar *mainBar;
    Gscreen *desktop;
    Gcontrol *controlFocused;
    Gcontrol *controlMouseMove;
};

#endif

