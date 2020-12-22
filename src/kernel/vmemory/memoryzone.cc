#include "memoryzone.h"
#include "../architecture/x86/x86.h"
extern Sx86 x86;

CmemoryZone::CmemoryZone(u32 amAddresStart, u32 amAddresEnd, u8* bitmap) : CmemoryBitmap(bitmap, (amAddresEnd - amAddresStart) / VM_PAGE_SIZE) {
    mAddresStart = amAddresStart;
    mAddresEnd = amAddresEnd;
}

u8* CmemoryZone::getPageToMemoryZone() {
    return (u8*) (mAddresStart + getPageToBitmap());
}

u8* CmemoryZone::getPageToMemoryZone(u32 countPages) {
    return (u8*) (mAddresStart + getPageToBitmap(countPages));
}

int CmemoryZone::relasePageToMemoryZone(u8* addressMemory, u32 countPages) {
    return relasePageToBitmap(addressMemory  - mAddresStart , countPages);
}

int CmemoryZone::relasePageToMemoryZone(u8* addressMemory) {
    if ((u32) addressMemory > mAddresEnd) {
        x86.ioScreen.printf("releasePageVirtual() Esta fuera de la memoria virtual /n");
        return -1;
    }
    relasePageToBitmap(addressMemory - mAddresStart);
}
