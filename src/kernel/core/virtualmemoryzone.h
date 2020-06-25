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

#ifndef VIRTUALMEMORYZONE_H
#define VIRTUALMEMORYZONE_H
#include "../defines/types.h"

class CvirtualMemoryZone {
public:
    CvirtualMemoryZone(u8 *abitmap, u32 avAddressStart, u32 avAddressEnd, u32 apageSize, u32 *adirectoryPage);
    u8 *getPageVirtualToBitmap(u32 *phisicalPage);
    int releasePageVirtual(u8 *avirtualAddress);

private:
    u32 vAddressStart;
    u32 vAddressEnd;
    u8 *bitmap;
    int size;
    int index;
    u8 bit;
    int count;
    u32 pageSize;
    u32 *directoryPage;
};

#endif

