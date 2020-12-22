#include "memorybitmap.h"
#include "../architecture/x86/virtualmemory.h"
#include "../architecture/x86/x86.h"

extern Sx86 x86;

CmemoryBitmap::CmemoryBitmap() {

}

CmemoryBitmap::CmemoryBitmap(u8* abitmap, u32 asize) {
    bitmap = abitmap;
    size = asize;
    occuped = 0;
    for (u32 i = 0; i < size / 8; i++)
        bitmap[i] = 0x0;
}

void CmemoryBitmap::setBitmap(u8* abitmap, u32 asize) {
    bitmap = abitmap;
    size = asize;
    occuped = 0;
    for (u32 i = 0; i < size / 8; i++)
        bitmap[i] = 0x0;
}

u8* CmemoryBitmap::getPageToBitmap() {
    int newPageAddress = -1;
    for (u32 i = 0; i < size / 8; i++) {
        if (bitmap[i] != 0xFF) {
            for (u32 j = 0; j < 8; j++) {
                if ((bitmap[i] & (1 << j)) == 0) {
                    newPageAddress = ((8 * i) + j) * VM_PAGE_SIZE;
                    bitmap[i] |= (1 << j);
                    occuped++;
                    return (u8*) newPageAddress;
                }
            }
        }
    }
    return (u8*) newPageAddress;
}

u8* CmemoryBitmap::getPageToBitmap(u32 countPages) {
    if (countPages >= 8)
        return getBitmapContiguosMay8(countPages);
    if (countPages < 8)
        return getBitmapContiguosMen8(countPages);
}

int CmemoryBitmap::relasePageToBitmap(u8* memoryAddress) {
    u32 indexBitmap = ((u32) memoryAddress & 0xFFFFF000) / VM_PAGE_SIZE;
    u32 indexBit = indexBitmap % 8;
    indexBitmap /= 8;
    bitmap[indexBitmap] ^= 1 << (indexBit);
    occuped--;
}

int CmemoryBitmap::relasePageToBitmap(u8* memoryAddress, int countPages) {
    u32 indexBitmap = ((u32) memoryAddress & 0xFFFFF000) / VM_PAGE_SIZE;
    u32 indexBit = indexBitmap % 8;
    indexBitmap /= 8;
    u32 byteCount = countPages / 8, bitCount = countPages % 8;
    int i = indexBitmap;
    for (i; i < (indexBitmap + byteCount); i++)
        bitmap[i] = 0x0;
    if (byteCount) {
        i++;
        indexBit = 0;
    }
    for (int k = indexBit; k < bitCount; k++)
        bitmap[i] ^= 1 << (k);
}

int CmemoryBitmap::initializedBitmap(u32 countPages) {
    u32 pagesComp = countPages / 8;
    u32 bitsComp = countPages % 8;
    int i;
    for (i = 0; i < pagesComp; i++)
        bitmap[i] = 0xFF;
    i++;
    for (int k = 0; k < bitsComp; k++)
        bitmap[i] |= (1 << k);
    occuped = countPages;
}

u32 CmemoryBitmap::getOcupped() {
    return occuped;
}

u8 * CmemoryBitmap::getBitmapContiguosMay8(u32 countPages) {
    u32 byteCount = countPages / 8, bitCount = countPages % 8;
    if (bitCount > 0) byteCount++;
    u32 byteFind = 0, startMemory = 0;
    for (int i = 0; i < size / 8; i++) {
        if (bitmap[i] == 0x0) {
            if (byteFind == 0) startMemory = i;
            byteFind++;
            if (byteFind == byteCount) {
                for (int k = startMemory; k < byteCount + startMemory; k++)
                    bitmap[k] = 0xFF;
                return (u8*) ((startMemory * 8) * VM_PAGE_SIZE);
            }
        } else byteFind = 0;
    }
    return (u8*) - 1;
}

u8 * CmemoryBitmap::getBitmapContiguosMen8(u32 countPages) {
    u32 bitFind = 0, startMemory, bitCount = countPages % 8;
    for (u32 i = 0; i < size / 8; i++) {
        if (bitmap[i] != 0xFF) {
            for (u32 j = 0; j < 8; j++) {
                if ((bitmap[i] & (1 << j)) == 0) {
                    if (bitFind == 0)
                        startMemory = j;
                    bitFind++;
                    if (bitFind == bitCount) {
                        for (int k = startMemory; k < bitCount; k++)
                            bitmap[i] |= (1 << k);
                        return (u8*) (((8 * i) + startMemory) * VM_PAGE_SIZE);
                    }
                } else
                    bitFind = 0;
            }
        }
    }
    return (u8*) - 1;
}
