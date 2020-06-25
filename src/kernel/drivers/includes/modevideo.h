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

#ifndef MODEVIDEO_H
#define MODEVIDEO_H

class CmodeVideo {
public:
    CmodeVideo();
    u32 height;
    u32 width;
    u32 fontSize;
    u32 mapFontSize;
    u8 SetReset;
    u8 EnableSetReset;
    u8 ColourCompare;
    u8 DataRotate;
    u8 ReadMapSelect;
    u8 GraphicsMode;
    u8 MiscellaneousGraphics;
    u8 ColourDontCare;
    u8 BitMask;
    u8 Reset;
    u8 ClockingMode;
    u8 MapMask;
    u8 CharacterMapSelect;
    u8 SequencerMemoryMode;
    u8 Palette0;
    u8 Palette1;
    u8 Palette2;
    u8 Palette3;
    u8 Palette4;
    u8 Palette5;
    u8 Palette6;
    u8 Palette7;
    u8 Palette8;
    u8 Palette9;
    u8 Palette10;
    u8 Palette11;
    u8 Palette12;
    u8 Palette13;
    u8 Palette14;
    u8 Palette15;
    u8 AttributeModeControl;
    u8 OverscanColour;
    u8 ColourPlaneEnable;
    u8 HorizontalPixelPanning;
    u8 ColourSelect;
    u8 HorizontalTotal;
    u8 EndHorizontalDisplay;
    u8 StartHorizontalBlanking;
    u8 EndHorizontalBlanking;
    u8 StartHorizontalRetrace;
    u8 EndHorizontalRetrace;
    u8 VerticalTotal;
    u8 Overflow;
    u8 PresetRowScan;
    u8 MaximumScanLine;
    u8 CursorStart;
    u8 CursorEnd;
    u8 StartAddressHigh;
    u8 StartAddressLow;
    u8 CursorLocationHigh;
    u8 CursorLocationLow;
    u8 VerticalRetraceStart;
    u8 VerticalRetraceEnd;
    u8 VerticalDisplayEnd;
    u8 Offset;
    u8 UnderlineLocation;
    u8 StartVerticalBlanking;
    u8 EndVerticalBlanking;
    u8 CRTCModeControl;
    u8 LineCompare;
    u8 MiscellaneousOutput;
private:

};

enum EmodeVideo {
    t80x25x16=1,g320x200x256=2, g720x480x4 = 3
};
enum EfontType {
    lucidaConsole =1,systemByte=2
};

#endif

