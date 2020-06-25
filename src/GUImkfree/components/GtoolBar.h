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

#ifndef GTOOLBAR_H
#define GTOOLBAR_H
#include "../basics/control.h"
#include "../basics/kgraphics.h"
#include "../basics/list.h"

class GtoolButton : public Gcontrol {
public:
    GtoolButton();
    GtoolButton(int x, int y, int width, int height);
    void initialized();
    void repaint();
    void refresh();
    void mouseMove(Gcontrol *sender, int x, int y);
    ~GtoolButton();
    Gcolor color;
private:
    int state;    
};

class GtoolButtons {
public:
    GtoolButtons();
    void addButton(GtoolButton *newToolButton);
    GtoolButton *operator[](int index);
    GtoolButton *getButton(int index);
    void Delete(GtoolButton *button);
    int count();
    ~GtoolButtons();
private:
    Clist *toolButtons;
};

class GtoolBar : public Gcontrol {
public:
    GtoolBar();
    GtoolBar(int x, int y, int width, int height);
    void initialized();
    void repaint();
    void refresh();
    void addToolButton(GtoolButton *newToolButton);
    ~GtoolBar();
    Gcolor color;
    GtoolButtons toolButtons;
private:

};

#endif

