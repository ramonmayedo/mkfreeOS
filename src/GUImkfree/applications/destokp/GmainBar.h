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

#ifndef GMAINBAR_H
#define GMAINBAR_H
#include "../../basics/control.h"
#include "../../basics/kgraphics.h"
#include "../../basics/list.h"

class GmainBarButton : public Gcontrol {
public:
    GmainBarButton();
    GmainBarButton(int x, int y, int width, int height);
    void initialized();
    void repaint();
    void refresh();
    void mouseMove(Gcontrol *sender, int x, int y);
    ~GmainBarButton();
    Gcolor color;
    int tag;
    int wsState;
private:
    int state;    
};

class GmainBarButtons {
public:
    GmainBarButtons();
    void addButton(GmainBarButton *newToolButton);
    GmainBarButton *operator[](int index);
    GmainBarButton *getButton(int index);
    void Delete(GmainBarButton *button);
    int count();
    ~GmainBarButtons();
private:
    Clist *toolButtons;
};

class GmainBar : public Gcontrol {
public:
    GmainBar();
    GmainBar(int x, int y, int width, int height);
    void initialized();
    void repaint();
    void refresh();
    void addToolButton(GmainBarButton *newToolButton);
    void deleteToolButton(GmainBarButton *toolButton);
    ~GmainBar();
    Gcolor color;
    GmainBarButtons toolButtons;
    GmainBarButton *activeButton;
private:

};

#endif

