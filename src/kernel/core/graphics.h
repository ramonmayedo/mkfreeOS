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

#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "../drivers/vga.h"
#include "../drivers/bxvga.h"

#define  INSTALL_VGA    0x0
#define  INSTALL_BOSCH  0x1

enum commandGPH {
    cgpChangeModeVideo = 1, cgpPaintArea = 2, cgpChangeModeVideoAdvanced = 3,
    cgpGetArea = 4
};

class Cgraphics {
public:
    Cgraphics();
    int setPixel(u32 aX, u32 aY, int acolor);
    int changeMode(int width, int height);
    int command(int acommand, int parameter1, int parameter2);
    void putc(int fgcolor, int bgcolor, const char c);
    int getPixel(u32 x, u32 y);
    void installDevice();
    void clearScreen();
private:
    u8* frameBuffer;
    int indexInstall;
    void *adapter;
    u32 width, height;
};

#endif

