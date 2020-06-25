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

#include "GcolumnButton.h"

GcolumnButton::GcolumnButton() {
    setClassName("GcolumnButton");
}

GcolumnButton::GcolumnButton(int x, int y, int width, int height) : Gcontrol(x, y, width, height){
    setClassName("GcolumnButton");
}

void GcolumnButton::repaint() {
    graphics->restoreZone();
    if (border)  graphics->paintBorder(1, Gcolor(0x00cacaca));
    graphics->backGroundColor(Gcolor(0xd4d4d4));
    text->print(caption.c_str(), 0x0, 0xd4d4d4);
    graphics->repaint((Gframe*) text);
    if (bstate == bsNormal) {
        Grect rbottom(0, 0, graphics->width(), 2);
        rbottom.alignBottom(graphics->trueRect);
        graphics->paint(rbottom, Gcolor(0x808080));
        Grect rleft(0, 0, 2, graphics->height());
        rleft.alignRight(graphics->trueRect);
        graphics->paint(rleft, Gcolor(0x808080));
    } else if (bstate == bsClicked) {
        Grect rleft(0, 0, 1, graphics->height());
        rleft.alignRight(graphics->trueRect);
    }
}

void GcolumnButton::clickPressDown(Gcontrol* sender, int x, int y) {
    bstate = bsClicked;
    refresh();
}

void GcolumnButton::clickPressUp(Gcontrol* sender, int x, int y) {
    bstate = bsNormal;
    refresh();
}

void GcolumnButton::refresh() {
    repaint();
    kpaintArea(*this);
}

void GcolumnButton::initialized() {
    bstate = bsNormal;
    border = bsSingle;
    graphics = new Gframe(left, top, width, height);
    text = new Gfont(4, 0, width, height-2);
    text->backGroundColor(0xd4d4d4);
}


GcolumnButton::~GcolumnButton() {
}

