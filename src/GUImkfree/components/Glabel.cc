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

#include "Glabel.h"

Glabel::Glabel() {
    setClassName("Glabel");
    font = 0;
}

Glabel::Glabel(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("Glabel");
    font = 0;
}

void Glabel::repaint() {
    if (graphics == 0) graphics = new Gframe(left, top, width, height);
    if (font == 0) font = new Gfont(0, 0, width, height);
    graphics->drawRect = graphics->trueRect;
    font->backGroundColor(style.backGroundColor);
    font->print(caption.c_str(), style.fontColor, style.backGroundColor);
    graphics->paint(font);
}

Glabel::~Glabel() {
    if (font) delete(font);
}

