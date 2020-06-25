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

#include "Gscreen.h"
#include "../../basics/kgraphics.h"

Gscreen::Gscreen() {
    setClassName("Gscreen");
}

void Gscreen::setResolution(int rwidth, int rheight) {
    left = 0;
    top = 0;
    width = rwidth;
    height = rheight;
    changeModeAdvance(1,rwidth,rheight);
    if (graphics) delete(graphics);
    graphics = new Gframe(left, top, width, height);
}

void Gscreen::setColorBackGround(int color) {
    graphics->backGroundColor(Gcolor(color));
}

void Gscreen::refresh(){
    kpaintArea(*this);
}

Gscreen::~Gscreen() {
    
}