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

#include "font.h"

extern char lucidaConsoleFont;

Gfont::Gfont(int x, int y, int width, int height) : Gframe(x, y, width, height) {
    font.height = 16;
    font.width = 8;
    spaceSeparator = 2;
    cwidth = width / (font.width + spaceSeparator);
    cheight = height / (font.height + spaceSeparator);
    wordWrap = true;
}

int Gfont::print(char* text, Gcolor color, Gcolor background) {
    char *ptr = text;
    int px = spaceSeparator;
    int py = px;
    int x = 0,y=0;
    while (*ptr) {
        if (x < cwidth) {
            iputc(px, py, color, background, *ptr);
            ptr++;
            px += font.width + spaceSeparator;
        };
        x++;
        if (x == cwidth && wordWrap == true) {
            y++;
            if (y == cheight) return 0;
            px = spaceSeparator;
            x=0;
            py += font.height + spaceSeparator;
        }
    }
}

int Gfont::printLine(int dx, char* text, int size, Gcolor color, Gcolor background) {
    char *ptr = text;
    int px = spaceSeparator + dx;
    int py = spaceSeparator;
    int x = px / (font.width + spaceSeparator);
    while (*ptr && size > 0) {
        if (x < cwidth) {
            iputc(px, py, color, background, *ptr);
            ptr++;
            px += font.width + spaceSeparator;
        } else return x;
        x++;
        size--;
    }
}

void Gfont::putc(int x, int y, Gcolor color, Gcolor background, const char c) {
    int px = spaceSeparator + x;
    int py = spaceSeparator + y;
    Grect sep(x, py, spaceSeparator, font.height);
    paint(sep, background);
    iputc(px, py, color, background, c);
}

void Gfont::iputc(int x, int y, Gcolor color, Gcolor background, const char c) {
    char i, j;
    char *fontMap = &lucidaConsoleFont;
    for (i = 0; i < font.height; i++)
        for (j = 0; j < font.width; j++) {
            paintPixel(x + j, y + i, ((fontMap[i + c * font.height] >> (font.width - j)) & 1) ? color : background);
        }
}