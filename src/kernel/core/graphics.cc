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

#include "graphics.h"
#include "../architecture/x86/x86.h"
extern Score core;
extern Sx86 x86;

Cgraphics::Cgraphics() {
    
}

void Cgraphics::installDevice() {
    indexInstall = 0;
    (Cbxvga*) adapter = new Cbxvga(); //Se intalla adaptador Virtual Bosch
    indexInstall = ((Cbxvga*) adapter)->installBosh();
    if (indexInstall == INSTALL_BOSCH) {
        frameBuffer = ((Cbxvga*) adapter)->getFrameBuffer();
        ((Cbxvga*) adapter)->setResolution(640, 480);
        return;
    }

    delete((Cbxvga*) adapter);
    (Cvga*) adapter = new Cvga(); //Se installa estandar VGA
    ((Cvga*) adapter)->setMode(t80x25x16);
    frameBuffer = ((Cvga*) adapter)->getFrameBuffer();
    indexInstall = INSTALL_VGA;
}

int Cgraphics::changeMode(int kwidth, int kheight) {
    if (indexInstall == INSTALL_BOSCH) {
        ((Cbxvga*) adapter)->setResolution(kwidth, kheight);
        return 0;
    }
    if (indexInstall == INSTALL_VGA) {
        ((Cvga*) adapter)->setMode(g320x200x256);
        return 0;
    }
}

int Cgraphics::command(int acommand, int parameter1, int parameter2) {
    switch (acommand) {
        case cgpChangeModeVideo:
        {
            return ((Cvga*) adapter)->setMode(parameter1);
        }
        case cgpChangeModeVideoAdvanced:
        {
            changeMode(parameter1, parameter2);
            break;
        }
        case cgpPaintArea:
        {
            SvideoArea *area = (SvideoArea*) parameter1;
            for (int i = 0; i < area->height; i++)
                for (int j = 0; j < area->width; j++)
                    setPixel((j + area->left), (i + area->top), area->area[i * area->width + j]);
            break;
        }
        case cgpGetArea:
        {
            SvideoArea *area = (SvideoArea*) parameter1;
            for (int i = 0; i < area->height; i++)
                for (int j = 0; j < area->width; j++)
                    area->area[i * area->width + j] = getPixel((j + area->left), (i + area->top));
            break;
        }

        default: return 0;
    }
}

void Cgraphics::putc(int fgcolor, int bgcolor, const char c) {
    if (indexInstall == INSTALL_VGA) {
        ((Cvga*) adapter)->putc(0x07, c);
        return;
    }
    if (indexInstall == INSTALL_BOSCH)
        ((Cbxvga*) adapter)->putc(0xFF, c);
}

int Cgraphics::setPixel(u32 x, u32 y, int c) {
    if (indexInstall == INSTALL_BOSCH)
        return ((Cbxvga*) adapter)->setPixel(x, y, c);
    if (indexInstall == INSTALL_VGA)
        return ((Cvga*) adapter)->setPixel(x, y, c);
}

int Cgraphics::getPixel(u32 x, u32 y) {
    if (indexInstall == INSTALL_BOSCH)
        return ((Cbxvga*) adapter)->getPixel(x, y);
    if (indexInstall == INSTALL_VGA)
        return ((Cvga*) adapter)->getPixel(x, y);
}

void Cgraphics::clearScreen() {
    if (indexInstall == INSTALL_BOSCH)
        return ((Cbxvga*) adapter)->clearScreen();
    if (indexInstall == INSTALL_VGA)
        return ((Cvga*) adapter)->clearScreen();   
}