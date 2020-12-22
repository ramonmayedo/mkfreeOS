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

#ifndef VGA_H
#define VGA_H
#include "../defines/types.h"
#include "includes/modevideo.h"
#include "vga/registerVGA.h"
#include "vga/videomode/mode320x200x256.h"
#include "vga/videomode/modetext80x25x16.h"
#include "vga/videomode/mode720x480x4.h"
#include "bxvga.h"

#define	RAM_SCREEN     0xB8000        //Direccion de la RAM de Video
#define SCREEN_SIZE    0xFA0	     
#define SCREEN_LIMIT   0xB8FA0
#define SIZE_LINE      0x4f


class Cvga {
public:
    Cvga();
    int setMode(int amode);
    void setFont(int afont);
    int  getWidth();
    int  getHeight();
    void setPixelVGA(int aX,int aY,int acolor);
    void clearDisplay();
    void setdefaultVGApalette(u32 *avga256_24bit);
    u32 * getdefaultVGApalette();
    void setBackGroundColor(u32 color);
    u8* getFrameBuffer();
    void putc(int attrb ,char c);
    int setPixel(u32 x, u32 y, int acolor);
    int getPixel(u32 x, u32 y);
    void clearScreen();
    void paintArea(SvideoArea *area, bool copyOrWrite);
protected:   
    void setConfiguration(CmodeVideo *aconfiguration);
    void writeRegisterSequency(CmodeVideo *aconfiguration);
    void BlankAndUnlock();
    void WriteCRTCRegisters(CmodeVideo *aconfiguration);
    void WriteGraphicsRegisters(CmodeVideo *aconfiguration);
    void WriteAttributeRegisters(CmodeVideo *aconfiguration);
    void UnblankAndLock();
    void SelectPlane(u8 aplane);  
    void setColorVGApalette24(u32 aindex,u32 color);
    void setFont(CmodeVideo *aconfiguration,u8 *afont);
    void scrollUp(int nLines);

    
private:
    CmodeVideo *configuration;
    u32 *vgaPalette;
    volatile u8 *frameBuffer;
    CregisterVGA registerVGA;
    bool BlankedAndUnlocked;
    CmodeText80x25x16 modeText80x25x16;
    Cmode320x200x256 mode320x200x256;
    Cmode720x480x4  mode720x480x4; 
    int mode;
};

#endif

