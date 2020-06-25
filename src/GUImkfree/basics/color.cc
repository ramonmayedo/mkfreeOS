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

#include "color.h"

Gcolor::Gcolor() {
    vcolor = 0;
}

Gcolor::Gcolor(int color) {
    vcolor = color;
}

Gcolor::Gcolor(char r, char g, char b) {
    vcolor = (0xff000000 | (r << 16) | (g << 8) | b);
}

int Gcolor::valColor() {
    return vcolor;
}

char Gcolor::red() {
    return (vcolor >> 16)&0xFF;
}

char Gcolor::blue() {
    return vcolor&0xFF;
}

char Gcolor::green() {
    return (vcolor >> 8)&0xFF;
}

char Gcolor::alpha() {
    return (vcolor >> 24)&0xFF;
}

void Gcolor::setcolor(int color) {
    vcolor = color;
}

void Gcolor::setred(char r) {
    vcolor = vcolor&0xFF00FFFF | (r << 16);
}

void Gcolor::setblue(char b) {
    vcolor = vcolor&0xFFFFFF00 | b;
}

void Gcolor::setgreen(char g) {
    vcolor = vcolor&0xFFFF00FF | (g << 8);
}

void Gcolor::setalpha(char a) {
    vcolor = vcolor&0x00FFFFFF | (a << 24);
}

Gcolor Gcolor::grayScale() {
    int grayValue = (red()+green()+blue())/3;
    return Gcolor(grayValue,grayValue,grayValue);
}

Gcolor Gcolor::darkned() {
    return Gcolor(red()/2,green()/2,blue()/2);
}

Gcolor Gcolor::lightned() {

}

Gcolor Gcolor::inverted() {
    return Gcolor(~red(), ~green(), ~blue());
}

Gcolor Gcolor::widthAlpha(char a) {
    return Gcolor((0x00FFFFFF & vcolor) | a << 24);
}

bool Gcolor::operator==(Gcolor& color) {
    return (vcolor==color.vcolor);
}

bool Gcolor::operator!=(Gcolor& color) {
    return (vcolor!=color.vcolor);
}



