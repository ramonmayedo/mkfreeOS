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

#include "mode720x480x4.h"

Cmode720x480x4::Cmode720x480x4() {
    height = 480;
    width = 720;
    fontSize = 0;
    mapFontSize = 0;
    //writeMiscellaneousOutput
    MiscellaneousOutput = 0xE7; // 0
    //writeRegisterSequency
    Reset = 0x03; // 0
    ClockingMode = 0x01; // 1
    MapMask = 0x08; // 2
    CharacterMapSelect = 0x00; // 3
    SequencerMemoryMode = 0x06; // 4
    //WriteCRTCRegisters
    HorizontalTotal = 0x6B; // 0
    EndHorizontalDisplay = 0x59; // 1
    StartHorizontalBlanking = 0x5A; // 2
    EndHorizontalBlanking = 0x82; // 3
    StartHorizontalRetrace = 0x60; // 4
    EndHorizontalRetrace = 0x8D; // 5
    VerticalTotal = 0x0B; // 6
    Overflow = 0x3E; // 7
    PresetRowScan = 0x00; // 8
    MaximumScanLine = 0x40; // 9
    CursorStart = 0x06; // A
    CursorEnd = 0x07; // B
    StartAddressHigh = 0x00; // C
    StartAddressLow = 0x00; // D
    CursorLocationHigh = 0x00; // E
    CursorLocationLow = 0x00; // F
    VerticalRetraceStart = 0xEA; // 10  
    VerticalRetraceEnd = 0x0C; // 11
    VerticalDisplayEnd = 0xDF; // 12
    Offset = 0x2D; // 13
    UnderlineLocation = 0x08; // 14
    StartVerticalBlanking = 0xE8; // 15 
    EndVerticalBlanking = 0x05; // 16
    CRTCModeControl = 0xE3; // 17  
    LineCompare = 0xFF; // 18
    //WriteGraphicsRegisters
    SetReset = 0x00; // 0
    EnableSetReset = 0x00; // 1
    ColourCompare = 0x00; // 2
    DataRotate = 0x00; // 3
    ReadMapSelect = 0x03; // 4
    GraphicsMode = 0x00; // 5
    MiscellaneousGraphics = 0x05; // 6
    ColourDontCare = 0x0F; // 7
    BitMask = 0xFF; // 8
    //WriteAttributeRegisters
    Palette0 = 0x00; // 0
    Palette1 = 0x01; // 1
    Palette2 = 0x02; // 2
    Palette3 = 0x03; // 3
    Palette4 = 0x04; // 4
    Palette5 = 0x05; // 5
    Palette6 = 0x06; // 6
    Palette7 = 0x07; // 7
    Palette8 = 0x08; // 8
    Palette9 = 0x09; // 9
    Palette10 = 0x0A; // A
    Palette11 = 0x0B; // B
    Palette12 = 0x0C; // C
    Palette13 = 0x0D; // D
    Palette14 = 0x0E; // E
    Palette15 = 0x0F; // F
    AttributeModeControl = 0x01; // 10
    OverscanColour = 0x00; // 11
    ColourPlaneEnable = 0x0F; // 12
    HorizontalPixelPanning = 0x00; // 13
    ColourSelect = 0x00; // 14
}


