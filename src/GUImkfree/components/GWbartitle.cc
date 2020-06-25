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

#include "GWbartitle.h"
#include "Gwindows.h"
#include "../systemWindows/Gclient.h"

void buttonMinimizedOnClickUp(Gcontrol *sender, int x, int y);
void buttonColoseOnClickUp(Gcontrol *sender, int x, int y);

GWbartitle::GWbartitle(){
    setClassName("GWbartitle");
}

GWbartitle::GWbartitle(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("GWbartitle");
    color = 0x00e6e6e6;
    initialized();
}

void GWbartitle::repaint() {
    text->print(caption.c_str(), 0x0, color);
    graphics->repaint((Gframe*) text);
}

GWbartitle::~GWbartitle() {
}

void GWbartitle::initialized() {
    graphics = new Gframe(0, 0, width, height);
    graphics->restoreZone();
    graphics->backGroundColor(color);
    graphics->reducedZone(4, 2);
    bClose = new Gbutton(0, 0, height, graphics->trueRect.height());
    bClose->caption = "X";
    bClose->parent = this;
    bClose->onClickUp = buttonColoseOnClickUp;
    bClose->repaint();
    bMinimized = new Gbutton(26, 0, height, graphics->trueRect.height());
    bMinimized->caption = "_";
    bMinimized->parent = this;
    bMinimized->onClickUp = buttonMinimizedOnClickUp;
    bMinimized->repaint();
    text = new Gfont(60, 2, 200, graphics->trueRect.height());
    text->backGroundColor(color);
    insertControl(bClose);
    insertControl(bMinimized);
    repaintAllControlChilds();
}

void buttonMinimizedOnClickUp(Gcontrol *sender, int x, int y) {
   Gwindows *windows = (Gwindows*) (sender->parent->parent);
   ((Gclient*)windows->application)->applicationCommand(cmsMinimizedWindows);
}

void buttonColoseOnClickUp(Gcontrol *sender, int x, int y) {
    Gwindows *windows = (Gwindows*) (sender->parent->parent);
    ((Gclient*) windows->application)->applicationCommand(cmsCloseWindows);
}