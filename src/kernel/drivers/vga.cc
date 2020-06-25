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

#include "vga.h"
#include "../architecture/x86/x86.h"

extern u8 lucidaConsoleFont;
extern u8 systemByteFont;
extern u32 difault_vga_palette;

extern Sx86 x86;

Cvga::Cvga() {
    mode = 0;
}

void Cvga::setConfiguration(CmodeVideo *aconfiguration) {
    configuration = aconfiguration;
    registerVGA.writeMiscellaneousOutput(configuration->MiscellaneousOutput);
    writeRegisterSequency(aconfiguration);
    registerVGA.writeCRTRegister(VerticalRetraceEndRegisterIndex, aconfiguration->VerticalRetraceEnd);
    WriteCRTCRegisters(aconfiguration);
    WriteGraphicsRegisters(aconfiguration);
    WriteAttributeRegisters(aconfiguration);
    clearDisplay();
    UnblankAndLock();
}

void Cvga::clearDisplay() {
    u8 *dir = (u8*) 0xA0000;
    int maxPixel = configuration->width * configuration->height;
    for (int i = 0; i < maxPixel; i++)
        dir[i] = 0x00;
}

void Cvga::writeRegisterSequency(CmodeVideo *aconfiguration) {
    registerVGA.writeRegisterSequency(ResetRegisterIndex, aconfiguration->Reset);
    registerVGA.writeRegisterSequency(ClockingModeRegisterIndex, aconfiguration->ClockingMode);
    registerVGA.writeRegisterSequency(MapMaskRegisterIndex, aconfiguration->MapMask);
    registerVGA.writeRegisterSequency(CharacterMapSelectRegisterIndex, aconfiguration->CharacterMapSelect);
    registerVGA.writeRegisterSequency(SequencerMemoryModeRegisterIndex, aconfiguration->SequencerMemoryMode);
}

void Cvga::WriteCRTCRegisters(CmodeVideo *aconfiguration) {
    registerVGA.writeCRTRegister(HorizontalTotalRegisterIndex, aconfiguration->HorizontalTotal);
    registerVGA.writeCRTRegister(EndHorizontalDisplayRegisterIndex, aconfiguration->EndHorizontalDisplay);
    registerVGA.writeCRTRegister(StartHorizontalBlankingRegisterIndex, aconfiguration->StartHorizontalBlanking);
    registerVGA.writeCRTRegister(EndHorizontalBlankingRegisterIndex, aconfiguration->EndHorizontalBlanking);
    registerVGA.writeCRTRegister(StartHorizontalRetraceRegisterIndex, aconfiguration->StartHorizontalRetrace);
    registerVGA.writeCRTRegister(EndHorizontalRetraceRegisterIndex, aconfiguration->EndHorizontalRetrace);
    registerVGA.writeCRTRegister(VerticalTotalRegisterIndex, aconfiguration->VerticalTotal);
    registerVGA.writeCRTRegister(OverflowRegisterIndex, aconfiguration->Overflow);
    registerVGA.writeCRTRegister(PresetRowScanRegisterIndex, aconfiguration->PresetRowScan);
    registerVGA.writeCRTRegister(MaximumScanLineRegisterIndex, aconfiguration->MaximumScanLine);
    registerVGA.writeCRTRegister(CursorStartRegisterIndex, aconfiguration->CursorStart);
    registerVGA.writeCRTRegister(CursorEndRegisterIndex, aconfiguration->CursorEnd);
    registerVGA.writeCRTRegister(StartAddressHighRegisterIndex, aconfiguration->StartAddressHigh);
    registerVGA.writeCRTRegister(StartAddressLowRegisterIndex, aconfiguration->StartAddressLow);
    registerVGA.writeCRTRegister(CursorLocationHighRegisterIndex, aconfiguration->CursorLocationHigh);
    registerVGA.writeCRTRegister(CursorLocationLowRegisterIndex, aconfiguration->CursorLocationLow);
    registerVGA.writeCRTRegister(VerticalRetraceStartRegisterIndex, aconfiguration->VerticalRetraceStart);
    //registerVGA.writeCRTRegister(VerticalRetraceEndRegisterIndex, averticalRetraceEndValue);
    registerVGA.writeCRTRegister(VerticalDisplayEndRegisterIndex, aconfiguration->VerticalDisplayEnd);
    registerVGA.writeCRTRegister(OffsetRegisterIndex, aconfiguration->Offset);
    registerVGA.writeCRTRegister(UnderlineLocationRegisterIndex, aconfiguration->UnderlineLocation);
    registerVGA.writeCRTRegister(StartVerticalBlankingRegisterIndex, aconfiguration->StartVerticalBlanking);
    registerVGA.writeCRTRegister(EndVerticalBlankingRegisterIndex, aconfiguration->EndVerticalBlanking);
    registerVGA.writeCRTRegister(CRTCModeControlRegisterIndex, aconfiguration->CRTCModeControl);
    registerVGA.writeCRTRegister(LineCompareRegisterIndex, aconfiguration->LineCompare);
}

void Cvga::WriteGraphicsRegisters(CmodeVideo *aconfiguration) {
    registerVGA.writeGraphicsControllerData(SetResetRegisterIndex, aconfiguration->SetReset);
    registerVGA.writeGraphicsControllerData(EnableSetResetRegisterIndex, aconfiguration->EnableSetReset);
    registerVGA.writeGraphicsControllerData(ColourCompareRegisterIndex, aconfiguration->ColourCompare);
    registerVGA.writeGraphicsControllerData(DataRotateRegisterIndex, aconfiguration->DataRotate);
    registerVGA.writeGraphicsControllerData(ReadMapSelectRegisterIndex, aconfiguration->ReadMapSelect);
    registerVGA.writeGraphicsControllerData(GraphicsModeRegisterIndex, aconfiguration->GraphicsMode);
    registerVGA.writeGraphicsControllerData(MiscellaneousGraphicsRegisterIndex, aconfiguration->MiscellaneousGraphics);
    registerVGA.writeGraphicsControllerData(ColourDontCareRegisterIndex, aconfiguration->ColourDontCare);
    registerVGA.writeGraphicsControllerData(BitMaskRegisterIndex, aconfiguration->BitMask);
}

void Cvga::WriteAttributeRegisters(CmodeVideo *aconfiguration) {
    registerVGA.writePalette(Palette0RegisterIndex, aconfiguration->Palette0);
    registerVGA.writePalette(Palette1RegisterIndex, aconfiguration->Palette1);
    registerVGA.writePalette(Palette2RegisterIndex, aconfiguration->Palette2);
    registerVGA.writePalette(Palette3RegisterIndex, aconfiguration->Palette3);
    registerVGA.writePalette(Palette4RegisterIndex, aconfiguration->Palette4);
    registerVGA.writePalette(Palette5RegisterIndex, aconfiguration->Palette5);
    registerVGA.writePalette(Palette6RegisterIndex, aconfiguration->Palette6);
    registerVGA.writePalette(Palette7RegisterIndex, aconfiguration->Palette7);
    registerVGA.writePalette(Palette8RegisterIndex, aconfiguration->Palette8);
    registerVGA.writePalette(Palette9RegisterIndex, aconfiguration->Palette9);
    registerVGA.writePalette(Palette10RegisterIndex, aconfiguration->Palette10);
    registerVGA.writePalette(Palette11RegisterIndex, aconfiguration->Palette11);
    registerVGA.writePalette(Palette12RegisterIndex, aconfiguration->Palette12);
    registerVGA.writePalette(Palette13RegisterIndex, aconfiguration->Palette13);
    registerVGA.writePalette(Palette14RegisterIndex, aconfiguration->Palette14);
    registerVGA.writePalette(Palette15RegisterIndex, aconfiguration->Palette15);
    registerVGA.writePalette(AttributeModeControlRegisterIndex, aconfiguration->AttributeModeControl);
    registerVGA.writePalette(OverscanColourRegisterIndex, OverscanColourRegisterIndex);
    registerVGA.writePalette(ColourPlaneEnableRegisterIndex, aconfiguration->ColourPlaneEnable);
    registerVGA.writePalette(HorizontalPixelPanningRegisterIndex, aconfiguration->HorizontalPixelPanning);
    registerVGA.writePalette(ColourSelectRegisterIndex, aconfiguration->ColourSelect);
}

void Cvga::BlankAndUnlock() {

}

void Cvga::UnblankAndLock() {
    registerVGA.ioReadByte(0x3C0 + 0x1A);
    registerVGA.ioWriteByte(0x3C0, 0x20);
}

void Cvga::SelectPlane(u8 aplane) {
    u8 plane = aplane & 0x3;
    registerVGA.writeGraphicsControllerData(ReadMapSelectRegisterIndex, plane);
    registerVGA.writeRegisterSequency(MapMaskRegisterIndex, 1 << plane);
}

void Cvga::setdefaultVGApalette(u32 *avga256_24bit) {
    vgaPalette = avga256_24bit;
    for (int index = 0; index < 256; index++) {
        int value = vgaPalette[index];
        registerVGA.vgaSetPalette(index, (value >> 18)&0x3f, (value >> 10)&0x3f, (value >> 2)&0x3f);
    }
}
u32 * Cvga::getdefaultVGApalette(){
    return vgaPalette;
}

void Cvga::setBackGroundColor(u32 color) {
    u8 *dir = (u8*) 0xA0000;
    for (int i = 0; i < configuration->width*configuration->height; i++) {
        dir[i] = color;
    }
}

void Cvga::setPixelVGA(int aX, int aY, int acolor) {
    u8 *dir = (u8*) 0xA0000;
    u32 offset = (aX + (configuration->height) * aY);
    dir[offset] = acolor;
}

void Cvga::setColorVGApalette24(u32 aindex, u32 color) {
    registerVGA.vgaSetPalette(aindex, (color >> 18)&0x3f, (color >> 10)&0x3f, (color >> 2)&0x3f);
}

void Cvga::setFont(CmodeVideo *aconfiguration, u8 *afont) {
    registerVGA.writeRegisterSequency(ResetRegisterIndex, 0x01); // seq reset
    registerVGA.writeRegisterSequency(MapMaskRegisterIndex, 0x04); // image plane 2
    registerVGA.writeRegisterSequency(SequencerMemoryModeRegisterIndex, 0x07); // disable odd/even in sequencer
    registerVGA.writeRegisterSequency(ResetRegisterIndex, 0x03); // seq reset 

    registerVGA.writeGraphicsControllerData(ReadMapSelectRegisterIndex, 0x02); // read select plane 2
    registerVGA.writeGraphicsControllerData(GraphicsModeRegisterIndex, 0x00); // odd/even disabled
    registerVGA.writeGraphicsControllerData(MiscellaneousGraphicsRegisterIndex, 0x00); // memory map select A0000h-BFFFFh

    for (int i = 0; i < aconfiguration->mapFontSize; i += 16)
        for (int j = 0; j < aconfiguration->fontSize; j++)
            ((u8*) 0xA0000)[2 * i + j] = afont[i + j];

    registerVGA.writeRegisterSequency(ResetRegisterIndex, 0x01); // seq reset            
    registerVGA.writeRegisterSequency(MapMaskRegisterIndex, 0x03); // image planes 0 and 1
    registerVGA.writeRegisterSequency(CharacterMapSelectRegisterIndex, 0x00); // character sets 0
    registerVGA.writeRegisterSequency(SequencerMemoryModeRegisterIndex, 0x02); // plain 64 kb memory layout, with odd/even for text      
    registerVGA.writeRegisterSequency(ResetRegisterIndex, 0x03); // seq reset         

    registerVGA.writeGraphicsControllerData(ColourCompareRegisterIndex, 0x00); // no color compare
    registerVGA.writeGraphicsControllerData(DataRotateRegisterIndex, 0x00); // no rotate
    registerVGA.writeGraphicsControllerData(ReadMapSelectRegisterIndex, 0x00); // read select plane 0
    registerVGA.writeGraphicsControllerData(GraphicsModeRegisterIndex, 0x10); // odd/even enable
    registerVGA.writeGraphicsControllerData(MiscellaneousGraphicsRegisterIndex, 0x0E); // memory map select: 0xb8000   

}

void Cvga::setFont(int afont) {
    EfontType font = (EfontType) afont;
    switch (font) {
        case systemByte:
        {
            setFont(configuration, &systemByteFont);
            break;
        }
        case lucidaConsole:
        {
            setFont(configuration, &lucidaConsoleFont);
            break;
        }
    }
}

int Cvga::setMode(int amode) {
    if (amode ==  mode) return amode;
    EmodeVideo modeV = (EmodeVideo) amode;
    switch (modeV) {
        case t80x25x16:
        {
            configuration = (CmodeVideo*) & modeText80x25x16;
            setConfiguration(configuration);
            setFont(systemByte);
            mode = amode;
            clearDisplay();
            x86.ioScreen.setX(0);x86.ioScreen.setY(0);
            break;
        }
        case g320x200x256:
        {
            configuration = (CmodeVideo*) & mode320x200x256;
            setConfiguration(configuration);
            setdefaultVGApalette(&difault_vga_palette);
            mode = amode;
            break;
        }
        case g720x480x4:
        {
            configuration = (CmodeVideo*) & mode720x480x4;
            setConfiguration(configuration);
            setdefaultVGApalette(&difault_vga_palette);
            clearDisplay();
            mode = amode;
            break;
        }
    }
    return mode;
}

int Cvga::getWidth() {
    return configuration->width;
}

int Cvga::getHeight() {
    return configuration->height;
}

u8* Cvga::getFrameBuffer() {
    return (u8*) 0xA0000;
}

void Cvga::putc(int attrb, char c) {
    u8 *ptrVideo;
    if (mode != t80x25x16)
        setMode(t80x25x16);

    ptrVideo = (u8*) (0xB8000 + 2 * x86.ioScreen.CurX + 160 * x86.ioScreen.CurY);

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
            break;
        }
        case CAR_BACKSPACE:
        { 
            if (x86.ioScreen.CurX > 0) {
                *(ptrVideo - 1) = 0x0;
                x86.ioScreen.CurX--;
            }
            break;
        }
        default:
        { //se imprime el caracter
            *ptrVideo = c;
            *(ptrVideo + 1) = attrb;
            x86.ioScreen.CurX++;
            if (x86.ioScreen.CurX > 79) {
                x86.ioScreen.CurX = 0;
                x86.ioScreen.CurY++;
            }
        }
    }
     if ( x86.ioScreen.CurY > 24) {scrollUp( x86.ioScreen.CurY-24); x86.ioScreen.CurY=24;}

}

int Cvga::setPixel(u32 x, u32 y, int acolor) {
    u32 offset;
    if (x > configuration->width || y > configuration->height) return 0;
    offset = (y * configuration->width) + x;
    u8 *buff = (u8*) (0xA0000);
    buff[offset] = acolor;
    return 0;
}

int Cvga::getPixel(u32 x, u32 y) {
    u32 offset;
    if (x > configuration->width || y > configuration->height) return 0;
    offset = (y * configuration->width) + x;
    u8 *buff = (u8*) (0xA0000);
    return buff[offset];
}

void Cvga::scrollUp(int nLines) {
    u8 *ptrStartScroll = (u8*) (RAM_SCREEN + nLines * 160); //Direccion de donde se incia el movimiento 80x2byte (attr+char)
    u8 *ptrVideo = (u8*) RAM_SCREEN; //Inicio de la RAM de video
    for (int i = 0; i < SCREEN_SIZE; i++) { //Se recorre la memoria de video
        if (ptrStartScroll < (u8*) (SCREEN_LIMIT)) {
            *(ptrVideo++) = *(ptrStartScroll); //Se mueve desde la linea x a la 0...
            *(ptrVideo) = 0x07;
        } else {
            *(ptrVideo++) = 0x0; //Se borran las ultimas x lineas
            *(ptrVideo) = 0x07;
        }
        ptrVideo++;
        ptrStartScroll += 2;
    }
}

void Cvga::clearScreen() {
    x86.ioScreen.CurX = 0;
    x86.ioScreen.CurY = 0;
    u8 *ptrVideo = (u8*) RAM_SCREEN;
    for (int i = 0; i < SCREEN_SIZE; i++)
        *(ptrVideo++) = 0x0;
}
