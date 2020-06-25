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

#include "bmp.h"
#include "../architecture/x86/x86.h"
#include "../drivers/vga.h"

extern Sx86 x86;
extern Score core;

Cbmp::Cbmp() {
}

int Cbmp::loadFromFile(char *afile) {
    file = *(SfileBMP*) afile;
    if (file.fileHeader.fType[0] != 'B' || file.fileHeader.fType[1] != 'M') return -1;
    /*  x86.ioScreen.printf("size %i /n",file.fileHeader.fSize);
      x86.ioScreen.printf("offset %i /n",file.fileHeader.fOffset);
      x86.ioScreen.printf("height %i /n",file.infoHeader.iHeight);
      x86.ioScreen.printf("width %i /n",file.infoHeader.iWidth); 
      x86.ioScreen.printf("imageSize %i /n",file.infoHeader.iImageSize); 
      x86.ioScreen.printf("color %i /n",file.infoHeader.iColors); 
      x86.ioScreen.printf("size %i /n",sizeof(file));*/
    // u32 *savePallete = core.vga->getdefaultVGApalette();
    int x = 0, y = file.infoHeader.iHeight;
    int rest = (file.infoHeader.iWidth % 4);
    if (rest != 0) rest = (file.infoHeader.iHeight % 4);
    char color;
    for (int i = 0; i < file.infoHeader.iImageSize; i++) {
        color = afile[file.fileHeader.fOffset + i];
        if (colorActivate == true) {
            if (backGroundColor == color) color = colorChange;
        }
        core.graphics.setPixel(x, y, color);
        x++;
        if (x == file.infoHeader.iWidth) {
            i += rest;
            y--;
            x = 0;
        }
    }
    return 0;
    // core.vga->setdefaultVGApalette(savePallete); 
}

Cbmp::~Cbmp() {
}

void Cbmp::setTransparentColor(char abackGroundColor, char acolorChange) {
    backGroundColor = abackGroundColor;
    colorChange = acolorChange;
    colorActivate = true;
}

void Cbmp::quitTransparentColor() {
    colorActivate = false;
}
