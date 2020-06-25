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

#include "registerVGA.h"
#include "../../architecture/x86/x86.h"

extern Sx86 x86;

CregisterVGA::CregisterVGA() {
    
}

void CregisterVGA::writeMiscellaneousOutput(u8 amiscellaneousOutput) {
    ioWriteByte(WriteMiscellaneousOutput, amiscellaneousOutput);
}

u8 CregisterVGA::readMiscellaneousOutput() {
    ioReadByte(ReadMiscellaneousOutput);
}

void CregisterVGA::writeCRTRegister(u8 aregisterIndex,u8 avalue){
    ioWriteByte(CRTControllerIndexColor, aregisterIndex);
    ioWriteByte(CRTControllerDataColor, avalue);
}

u8 CregisterVGA::readCRTRegister(u8 aregisterIndex){
   ioWriteByte(CRTControllerIndexColor, aregisterIndex);
   return ioReadByte(CRTControllerDataColor);   
}

void CregisterVGA::writeRegisterSequency(u8 aregisterIndex,u8 avalue){
    ioWriteByte(SequencerIndex, aregisterIndex);
    ioWriteByte(SequencerData, avalue); 
}

u8 CregisterVGA::readRegisterSequency(u8 aregisterIndex) {
    ioWriteByte(SequencerIndex, aregisterIndex);
    return ioReadByte(SequencerData);
}

void CregisterVGA::writeGraphicsControllerData(u8 aregisterIndex, u8 avalue) {
    ioWriteByte(GraphicsControllerIndex, aregisterIndex);
    ioWriteByte(GraphicsControllerData, avalue);
}

u8 CregisterVGA::readGraphicsControllerData(u8 aregisterIndex) {
    ioWriteByte(GraphicsControllerIndex, aregisterIndex);
    return ioReadByte(GraphicsControllerData);
}

void CregisterVGA::writePalette(u8 aregisterIndex, u8 avalue) {
     ioReadByte(FeatureControlWriteColour);
     ioWriteByte(AttributeControllerWriteIndexData, aregisterIndex);
     ioWriteByte(AttributeControllerWriteIndexData, avalue);
}

u8 CregisterVGA::readPalette(u8 aregisterIndex) {
    ioReadByte(FeatureControlWriteColour);
    ioWriteByte(AttributeControllerWriteIndexData, aregisterIndex);
    return ioReadByte(AttributeControllerReadData);
}

void CregisterVGA::setDACMask(u8 aDACMask) {
    ioWriteByte(DACMask, aDACMask);
}

u8 CregisterVGA::getDACMask() {
    return ioReadByte(DACMask);
}

void CregisterVGA::vgaSetPalette(int index, int r, int g, int b) {
    ioWriteByte(0x3C8, index);
    ioWriteByte(0x3C9, r);
    ioWriteByte(0x3C9, g);
    ioWriteByte(0x3C9, b);
}


u8 CregisterVGA::ioReadByte(u32 aport) {
    return x86.port.inb(aport);
}

void CregisterVGA::ioWriteByte(u32 aport, u8 abyte) {
    x86.port.outb(aport, abyte);
}

