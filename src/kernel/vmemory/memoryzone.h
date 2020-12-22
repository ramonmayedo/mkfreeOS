#ifndef MEMORYZONE_H
#define MEMORYZONE_H
#include "memorybitmap.h"

class CmemoryZone : CmemoryBitmap {
public:
    CmemoryZone(u32 amAddresStart, u32 amAddresEnd, u8* bitmap);
    u8* getPageToMemoryZone();
    u8* getPageToMemoryZone(u32 countPages);
    int relasePageToMemoryZone(u8 *addressMemory);
    int relasePageToMemoryZone(u8 *addressMemory, u32 countPages);
private:
    u32 mAddresStart;
    u32 mAddresEnd;
};

#endif

