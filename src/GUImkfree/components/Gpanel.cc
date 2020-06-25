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

#include "Gpanel.h"

Gpanel::Gpanel() {
    setClassName("Gpanel");
}

Gpanel::Gpanel(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("Gpanel");
    graphics = new Gframe(left, top, width, height);
    padding=0;
}

void Gpanel::repaint() {
    if (border) graphics->paintBorder(1, Gcolor(0x0));
    graphics->backGroundColor(Gcolor(0xd4d4d4));
    graphics->reducedZone(padding, padding);
    repaintAllControlChilds();
}

Gpanel::~Gpanel() {
    delete(graphics);
}

