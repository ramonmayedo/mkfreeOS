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

#ifndef FONT_H
#define FONT_H
#include "graphicsbitmap.h"
#include "frame.h"

struct Sfont {
    int width;
    int height;
};

enum eTextAlign {
    alignCenter = 1, alignLeft = 2, alignRight = 3,
    alignTop = 4
};

class Gfont : public Gframe {
public:
    Gfont(int x, int y, int width, int height);
    int print(char *text, Gcolor color, Gcolor background);
    int printLine(int dx, char *text, int size, Gcolor color, Gcolor background);
    void putc(int x, int y, Gcolor color, Gcolor background, const char c);
    bool wordWrap;
private:
    int spaceSeparator;
    int cwidth;
    int cheight;
    Sfont font;
protected:
    void iputc(int x, int y, Gcolor color, Gcolor background, const char c);

};

#endif

