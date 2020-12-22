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

#include "bxvga.h"
#include "../architecture/x86/x86.h"
#include "../core/includes/templates.h"

extern Score core;
extern Sx86 x86;
extern u8 lucidaConsoleFont;

Cbxvga::Cbxvga() {
}

void Cbxvga::boshWrite(u16 index, u16 data) {
    x86.port.outw(VBE_DISPI_IOPORT_INDEX, index);
    x86.port.outw(VBE_DISPI_IOPORT_DATA, data);
}

u16 Cbxvga::boshRead(u16 index) {
    x86.port.outw(VBE_DISPI_IOPORT_INDEX, index);
    return x86.port.inb(VBE_DISPI_IOPORT_DATA);
}

void Cbxvga::boshDisabled() {
    boshWrite(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
}

void Cbxvga::boshEnabled() {
    boshWrite(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED | VBE_DISPI_LFB_ENABLED);
}

void Cbxvga::setResolution(u16 nwidth, u16 nheight) {
    boshDisabled();
    boshWrite(VBE_DISPI_INDEX_XRES, nwidth);
    boshWrite(VBE_DISPI_INDEX_YRES, nheight);
    boshWrite(VBE_DISPI_INDEX_BPP, 32);
    boshEnabled();
    boshWrite(VBE_DISPI_INDEX_VIRT_WIDTH, nwidth / 2);
    boshWrite(VBE_DISPI_INDEX_VIRT_HEIGHT, nheight / 2);
    width = nwidth;
    height = nheight;
    fontWidth = 8;
    fontHeight = 16;
    charWidth = (width / fontWidth) - 2;
    charHeight = (height / fontHeight) - 1;
}

int Cbxvga::installBosh() {
    if (!core.pci.find(BOCHS_VBE_VENDOR, BOCHS_VBE_DEVICE, &bus, &device, &function))
        return 0;
    frameBuffer = (u8*) core.pci.read(bus, device, function, PCI_BAR0); //4MB RAM
    CmemoryRegion *region = new CmemoryRegion((u32) frameBuffer, (u32) frameBuffer, 1024, false);
    region->setMappedKernel(WM_PG_CACHE_DISABLED);
    x86.virtualMemory.addRegion(region);
    return 1;
}

u8* Cbxvga::getFrameBuffer() {
    return frameBuffer;
}

void Cbxvga::putc(int attrb, char c) {
    int posX = x86.ioScreen.CurX *fontWidth;
    int posY = x86.ioScreen.CurY *fontHeight;
    switch (c) {
        case CAR_JMPLINE:
        { //caracter para salto de linea
            x86.ioScreen.CurX = 0;
            x86.ioScreen.CurY++;
            break;
        }
        case CAR_RETCURSOR:
        { //caracter para retorno del carro
            x86.ioScreen.CurX = 0;
            return;
        }
        case CAR_BACKSPACE:
        {
            if (x86.ioScreen.CurX > 0) {
                c = ' ';
                x86.ioScreen.CurX--;
                posX = x86.ioScreen.CurX *fontWidth;
            }
            break;
        }
        default:
        { //se imprime el caracter
            x86.ioScreen.CurX++;
            if (x86.ioScreen.CurX > charWidth) {
                x86.ioScreen.CurX = 0;
                x86.ioScreen.CurY++;
            }
        }
    }

    if (x86.ioScreen.CurY > charHeight) {
        scrollUp(fontHeight);
        x86.ioScreen.CurY = charHeight;
        return;
    }
    if (c == CAR_JMPLINE) return;

    u8 i, j;
    u8 *font = &lucidaConsoleFont;

    for (i = 0; i < fontWidth; i++)
        for (j = 0; j < fontHeight; j++) {
            setPixel(posX + i, posY + j, ((font[j + c * 16] >> fontWidth - i) & 1) ? attrb : 0x0);
        }
}

int Cbxvga::setPixel(u32 x, u32 y, int acolor) {
    u32 offset;
    if (x > width || y > height) return 0;
    offset = (y * width) + x;
    u32 *buff = (u32*) (frameBuffer);
    buff[offset] = acolor;
    return 0;
}

int Cbxvga::getPixel(u32 x, u32 y) {
    u32 offset;
    if (x > width || y > height) return 0;
    offset = (y * width) + x;
    u32 *buff = (u32*) (frameBuffer);
    return buff[offset];
}

void Cbxvga::scrollUp(int nLines) {
    u32 *ptrVideo = (u32*) frameBuffer; //Inicio de la RAM de video
    u32 screenSize = width*height;
    int scroll = nLines*width;
    u32 *ptrMove = ptrVideo + scroll;

    for (int i = 0; i < screenSize; i++) //Se recorre la memoria de video
        ptrVideo[i] = ptrMove[i];
}

void Cbxvga::clearScreen() {
    x86.ioScreen.CurX = 0;
    x86.ioScreen.CurY = 0;
    u32 *ptrVideo = (u32*) frameBuffer;
    u32 screenSize = width*height;
    for (int i = 0; i < screenSize; i++)
        ptrVideo[i] = 0x0;
}

void Cbxvga::paintArea(SvideoArea* area, bool copyOrWrite) {
    u32 *linearBufferDest = (u32*) frameBuffer + area->left + area->top * width;
    u32 *linearBufferSource = (u32*) area->area + area->trueLeft + area->trueTop * area->trueWidth;
    if (copyOrWrite == false) {
        for (int i = 0; i < area->height; i++) {
            fastCopy32(linearBufferDest, linearBufferSource, area->width);
            linearBufferDest += width;
            linearBufferSource += area->trueWidth;
        }
    } else {
        for (int i = 0; i < area->height; i++) {
            fastCopy32(linearBufferSource, linearBufferDest, area->width);
            linearBufferDest += width;
            linearBufferSource += area->trueWidth;
        }
    }
}
