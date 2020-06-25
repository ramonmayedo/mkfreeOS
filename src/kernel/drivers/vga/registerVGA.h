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

#ifndef REGISTERVGA_H
#define REGISTERVGA_H
#include "../../defines/types.h"

//Puertos de Entrada Salida Para VGA

#define WriteMiscellaneousOutput 0x3C2
#define ReadMiscellaneousOutput  0x3CC

#define GraphicsControllerIndex  0x3CE
#define GraphicsControllerData   0x3CF

#define SequencerIndex 0x3C4
#define SequencerData 0x3C5

#define CRTControllerIndexColor  0x3D4
#define CRTControllerDataColor  0x3D5

#define FeatureControlWriteColour 0x3DA
#define AttributeControllerWriteIndexData 0x3C0
#define AttributeControllerReadData 0x3C1

#define DACMask 0x3C6
//definiciones registros de sequencia
#define ResetRegisterIndex 0x00
#define ClockingModeRegisterIndex   0x01
#define MapMaskRegisterIndex  0x02
#define CharacterMapSelectRegisterIndex 0x03
#define SequencerMemoryModeRegisterIndex 0x04


//Region para los Registros CRT
#define HorizontalTotalRegisterIndex 0x00
#define EndHorizontalDisplayRegisterIndex 0x01
#define StartHorizontalBlankingRegisterIndex 0x02
#define EndHorizontalBlankingRegisterIndex  0x03
#define StartHorizontalRetraceRegisterIndex 0x04
#define EndHorizontalRetraceRegisterIndex 0x05
#define VerticalTotalRegisterIndex 0x06
#define OverflowRegisterIndex 0x07
#define PresetRowScanRegisterIndex 0x08
#define MaximumScanLineRegisterIndex 0x09
#define CursorStartRegisterIndex 0x0A
#define CursorEndRegisterIndex 0x0B
#define StartAddressHighRegisterIndex 0x0C
#define StartAddressLowRegisterIndex 0x0D
#define CursorLocationHighRegisterIndex 0x0E
#define CursorLocationLowRegisterIndex 0x0F
#define VerticalRetraceStartRegisterIndex 0x10
#define VerticalRetraceEndRegisterIndex 0x11
#define VerticalDisplayEndRegisterIndex 0x12
#define OffsetRegisterIndex 0x13
#define UnderlineLocationRegisterIndex 0x14
#define StartVerticalBlankingRegisterIndex 0x15
#define EndVerticalBlankingRegisterIndex 0x16
#define CRTCModeControlRegisterIndex 0x17
#define LineCompareRegisterIndex 0x18

//registro de modo grafico
#define SetResetRegisterIndex 0x00
#define EnableSetResetRegisterIndex 0x01
#define ColourCompareRegisterIndex 0x02
#define DataRotateRegisterIndex 0x03
#define ReadMapSelectRegisterIndex 0x04
#define GraphicsModeRegisterIndex 0x05
#define MiscellaneousGraphicsRegisterIndex 0x06
#define ColourDontCareRegisterIndex 0x07
#define BitMaskRegisterIndex 0x08

//Registro de atributod
#define Palette0RegisterIndex  0x0
#define Palette1RegisterIndex  0x1
#define Palette2RegisterIndex  0x2
#define Palette3RegisterIndex  0x3
#define Palette4RegisterIndex  0x4
#define Palette5RegisterIndex  0x5
#define Palette6RegisterIndex  0x6
#define Palette7RegisterIndex  0x7
#define Palette8RegisterIndex  0x8
#define Palette9RegisterIndex  0x9
#define Palette10RegisterIndex 0xA
#define Palette11RegisterIndex 0xB
#define Palette12RegisterIndex 0xC
#define Palette13RegisterIndex 0xD
#define Palette14RegisterIndex 0xE
#define Palette15RegisterIndex 0xF
#define AttributeModeControlRegisterIndex 0x10
#define OverscanColourRegisterIndex 0x11
#define ColourPlaneEnableRegisterIndex 0x12
#define HorizontalPixelPanningRegisterIndex 0x13
#define ColourSelectRegisterIndex 0x14

class CregisterVGA {
public:
    CregisterVGA();
    void writeMiscellaneousOutput(u8 amiscellaneousOutput);
    u8 readMiscellaneousOutput();
    void writeRegisterSequency(u8 aregisterIndex, u8 avalue);
    u8 readRegisterSequency(u8 aregisterIndex);
    void writeCRTRegister(u8 aregisterIndex, u8 avalue);
    u8 readCRTRegister(u8 aregisterIndex);
    void writeGraphicsControllerData(u8 aregisterIndex, u8 avalue);
    u8 readGraphicsControllerData(u8 aregisterIndex);
    void writePalette(u8 aregisterIndex, u8 avalue);
    u8 readPalette(u8 aregisterIndex);
    void setDACMask(u8 aDACMask);
    u8 getDACMask();
    void vgaSetPalette(int index, int r, int g, int b);
    u8 ioReadByte(u32 aport);
    void ioWriteByte(u32 aport, u8 abyte);
};

#endif