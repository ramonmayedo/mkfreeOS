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

#ifndef BXVGA_H
#define BXVGA_H
#include "../defines/types.h"

#define BOCHS_VBE_VENDOR  0x1234
#define BOCHS_VBE_DEVICE  0x1111

#define VBE_DISPI_IOPORT_INDEX  0x1CE
#define VBE_DISPI_IOPORT_DATA   0x1CF

#define VBE_DISPI_INDEX_XRES        1
#define VBE_DISPI_INDEX_YRES        2
#define VBE_DISPI_INDEX_BPP         3
#define VBE_DISPI_INDEX_ENABLE      4
#define VBE_DISPI_INDEX_BANK        5
#define VBE_DISPI_INDEX_VIRT_WIDTH  6
#define VBE_DISPI_INDEX_VIRT_HEIGHT 7
#define VBE_DISPI_INDEX_Y_OFFSET    9
// flags for VBE_DISPI_INDEX_ENABLE
#define VBE_DISPI_DISABLED          0x00
#define VBE_DISPI_ENABLED           0x01
#define VBE_DISPI_LFB_ENABLED       0x40
#define VBE_DISPI_NOCLEARMEM        0x80

#define PCI_BAR0    0x10
#define PCI_BAR1    0x14
#define PCI_BAR2    0x18
#define PCI_BAR3    0x1C
#define PCI_BAR4    0x20
#define PCI_BAR5    0x24

struct SvideoArea {
    int *area;
    int left;
    int top;
    int height;
    int width;
    int trueLeft, trueTop, trueHeight, trueWidth;
};

struct SvideoAreaTrue {
    int *area;
    int left;
    int top;
    int height;
    int width;
    int heightTrue;
    int widthTrue;
    
};

class Cbxvga {
public:
    Cbxvga();
    void boshWrite(u16 index, u16 data);
    u16 boshRead(u16 index);
    void boshEnabled();
    void boshDisabled();
    void setResolution(u16 width, u16 height);
    int installBosh();
    u8* getFrameBuffer();
    int setPixel(u32 x, u32 y, int acolor);
    int getPixel(u32 x, u32 y);
    void putc(int attrb, char c);
    void scrollUp(int nLines);
    void clearScreen();
    void paintArea(SvideoArea *area,bool copyOrWrite);
    
private:
    u32 bus, device, function;
    u8 *frameBuffer;
    u32 width, height;
    u8 fontWidth, fontHeight;
    u16 charWidth;
    u16 charHeight;
};

#endif

