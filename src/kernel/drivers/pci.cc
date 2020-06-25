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

#include "pci.h"
#include "../architecture/x86/x86.h"

extern Score core;
extern Sx86 x86;

Cpci::Cpci() {
}

u32 Cpci::read(u32 bus, u32 device, u32 function, u32 offset) {
    u32 reg = 0x80000000;
    reg |= (bus & 0xFF) << 16;
    reg |= (device & 0x1F) << 11;
    reg |= (function & 0x7) << 8;
    reg |= (offset & 0xFF) & 0xFC;
    x86.port.outl(PCI_CONFIG, reg);
    return x86.port.inl(PCI_DATA);
}

void Cpci::write(u32 bus, u32 device, u32 function, u32 offset, u32 data) {
    u32 reg = 0x80000000;
    reg |= (bus & 0xFF) << 16;
    reg |= (device & 0x1F) << 11;
    reg |= (function & 0x7) << 8;
    reg |= offset & 0xFC;
    x86.port.outl(PCI_CONFIG, reg);
    x86.port.outl(PCI_DATA, data);
}

void Cpci::test() {
    u32 vend_dev, b, d, f;
    x86.ioScreen.printf("Devices over PCI:/n");
    for (b = 0; b < 256; b++)
        for (d = 0; d < 32; d++)
            for (f = 0; f < 8; f++) {
                vend_dev = read(b, d, f, PCI_VENDOR_DEVICE);
                if (vend_dev != 0xFFFFFFFF)
                    x86.ioScreen.printf("[%i:%i.%i] %i:%i/n", b, d, f, vend_dev & 0xFFFF, vend_dev >> 16);
            }
}

u8 Cpci::find(u32 vendor, u32 device, u32* bus, u32* dev, u32* function) {
    u32 vend_dev, b, d, f, my_vend_dev;
    my_vend_dev = (vendor & 0xFFFF) | (device << 16);

    for (b = 0; b < 256; b++)
        for (d = 0; d < 32; d++)
            for (f = 0; f < 8; f++) {
                vend_dev = read(b, d, f, PCI_VENDOR_DEVICE);
                if (vend_dev == my_vend_dev) {
                    *bus = b;
                    *dev = d;
                    *function = f;
                    return true;
                }
            }
    return false;
}