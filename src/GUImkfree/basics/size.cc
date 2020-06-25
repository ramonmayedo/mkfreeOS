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

#include "size.h"

Gsize::Gsize() {
    sheight = 0;
    swidth = 0;
}

Gsize::Gsize(int width, int height) {
    swidth = width;
    sheight = height;
}

int Gsize::height() {
    return sheight;
}

int Gsize::width() {
    return swidth;
}

void Gsize::setwidth(int width) {
    swidth = width;
}

void Gsize::setheight(int height) {
    sheight = height;
}

bool Gsize::operator==(Gsize& size) {
    return (sheight == size.sheight && swidth == size.width());
}

Gsize Gsize::operator-(Gsize& size) {
    Gsize nsize;
    nsize.swidth = swidth - size.swidth;
    nsize.sheight = sheight - size.height();
    return nsize;
}

Gsize& Gsize::operator-=(Gsize& size) {
    swidth -= size.swidth;
    sheight -= size.sheight;
    return *this;
}

Gsize Gsize::operator+(Gsize& size) {
    Gsize nsize;
    nsize.swidth = swidth + size.swidth;
    nsize.sheight = sheight + size.height();
    return nsize;
}

Gsize& Gsize::operator+=(Gsize& size) {
    swidth += size.swidth;
    sheight += size.sheight;
    return *this;
}

