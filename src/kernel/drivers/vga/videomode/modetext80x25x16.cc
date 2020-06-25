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

#include "modetext80x25x16.h"

CmodeText80x25x16::CmodeText80x25x16() {
    height = 80;
    width  = 25;
    fontSize=16;
    mapFontSize=4096;
    //writeMiscellaneousOutput
    MiscellaneousOutput = 0x67;    // 0
    //writeRegisterSequency
    Reset = 0x03;                  // 0
    ClockingMode = 0x00;           // 1
    MapMask = 0x03;                // 2
    CharacterMapSelect = 0x00;     // 3
    SequencerMemoryMode = 0x02;    // 4
    //WriteCRTCRegisters
    HorizontalTotal = 0x5F;         // 0
    EndHorizontalDisplay = 0x4F;    // 1
    StartHorizontalBlanking = 0x50; // 2
    EndHorizontalBlanking = 0x82;   // 3
    StartHorizontalRetrace = 0x55;  // 4
    EndHorizontalRetrace = 0x81;    // 5
    VerticalTotal = 0xBF;           // 6
    Overflow = 0x1F;                // 7
    PresetRowScan = 0x00;           // 8
    MaximumScanLine = 0x4F;         // 9
    CursorStart = 0x20;             // A
    CursorEnd = 0x0e;               // B
    StartAddressHigh = 0x00;        // C
    StartAddressLow = 0x00;         // D
    CursorLocationHigh = 0x01;      // E
    CursorLocationLow = 0xe0;       // F
    VerticalRetraceStart = 0x9C;    // 10  
    VerticalRetraceEnd = 0x8E;      // 11
    VerticalDisplayEnd = 0x8F;      // 12
    Offset = 0x28;                  // 13
    UnderlineLocation = 0x1F;       // 14
    StartVerticalBlanking = 0x96;   // 15 
    EndVerticalBlanking = 0xB9;     // 16
    CRTCModeControl = 0xA3;         // 17  
    LineCompare = 0xFF;             // 18
    //WriteGraphicsRegisters
    SetReset = 0x00;               // 0
    EnableSetReset = 0x00;         // 1
    ColourCompare = 0x00;          // 2
    DataRotate = 0x00;             // 3
    ReadMapSelect = 0x00;          // 4
    GraphicsMode = 0x10;           // 5
    MiscellaneousGraphics = 0x0E;  // 6
    ColourDontCare = 0x00;         // 7
    BitMask = 0xFF;                // 8
    //WriteAttributeRegisters
    Palette0 = 0x00;               // 0
    Palette1 = 0x01;               // 1
    Palette2 = 0x02;               // 2
    Palette3 = 0x03;               // 3
    Palette4 = 0x04;               // 4
    Palette5 = 0x05;               // 5
    Palette6 = 0x14;               // 6
    Palette7 = 0x07;               // 7
    Palette8 = 0x38;               // 8
    Palette9 = 0x39;               // 9
    Palette10 = 0x3A;              // A
    Palette11 = 0x3B;              // B
    Palette12 = 0x3C;              // C
    Palette13 = 0x3D;              // D
    Palette14 = 0x3E;              // E
    Palette15 = 0x3F;              // F
    AttributeModeControl = 0x0C;   // 10
    OverscanColour = 0x00;         // 11
    ColourPlaneEnable = 0x0F;      // 12
    HorizontalPixelPanning = 0x08; // 13
    ColourSelect = 0x00;           // 14
}
