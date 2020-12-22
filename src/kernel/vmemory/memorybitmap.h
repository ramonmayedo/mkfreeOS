#ifndef MEMORYBITMAP_H
#define MEMORYBITMAP_H

#include "../defines/types.h"


class CmemoryBitmap {
public:
    CmemoryBitmap();
    CmemoryBitmap(u8 *abitmap, u32 asize);
    void setBitmap(u8 *abitmap, u32 asize);
    u8 *getPageToBitmap();
    u8 *getPageToBitmap(u32 countPages);
    int relasePageToBitmap(u8* memoryAddress);
    int relasePageToBitmap(u8* memoryAddress, int countPages);
    int initializedBitmap(u32 countPages);
    u32 getOcupped();
private:
    u8 *bitmap;
    u32 size;
    u32 occuped;
    u8 * getBitmapContiguosMen8(u32 countPages);
    u8 * getBitmapContiguosMay8(u32 countPages);
};
#endif

