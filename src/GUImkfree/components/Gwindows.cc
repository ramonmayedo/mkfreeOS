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

#include "Gwindows.h"
#include "GWbartitle.h"

Gwindows::Gwindows() {
    setClassName("Gwindows");
}

Gwindows::Gwindows(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("Gwindows");
    initialized();
}

void Gwindows::repaint() {
    graphics->restoreZone();
    graphics->paintBorder(1, Gcolor(0x0));
    graphics->backGroundColor(Gcolor(0xd4d4d4));
    bartitle->caption = caption;
    bartitle->repaint();
    repaintAllControlChilds();
}

void Gwindows::refresh() {
    repaint();
    kpaintArea(*this);
}

void Gwindows::activate() {
    kpaintArea(*this);
}

void Gwindows::initialized() {
    controlFocused = 0;
    controlMouseMove = 0;
    graphics = new Gframe(left, top, width, height);
    bartitle = new GWbartitle(0, 0, width, 25);
    bartitle->parent = this;
    insertControl(bartitle);
}

Gwindows::~Gwindows() {
}

