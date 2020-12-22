#include "memoryregion.h"
#include "../architecture/x86/x86.h"
extern Sx86 x86;

CmemoryRegion::CmemoryRegion(u32 avAddresStart, u32 apAddresStart, u32 asize, bool aonlyKernel) {
    vAddresStart = avAddresStart;
    pAddresStart = apAddresStart;
    size = asize;
    onlyKernel = aonlyKernel;
}

void CmemoryRegion::setShared(bool shared) {
    onlyKernel = shared;
}

void CmemoryRegion::setMappedKernel(u32 flags) {
    u8* virtualAdd = (u8*) vAddresStart;
    for (int i = 0; i < size; i++) {
        x86.virtualMemory.addDirectoryPagesKernel(virtualAdd, virtualAdd, flags);
        virtualAdd += VM_PAGE_SIZE;
    }
    mappedKernel = true;
}

void CmemoryRegion::setMappedKernelPhisV(u32 flags) {
    u8* virtualAdd = (u8*) vAddresStart;
    u8* phis;
    for (int i = 0; i < size; i++) {
        phis = x86.virtualMemory.getPhisicalPage();
        x86.virtualMemory.addDirectoryPagesKernel(virtualAdd, phis , flags);
        virtualAdd += VM_PAGE_SIZE;
    }
    mappedKernel = true;
}

void CmemoryRegion::setMappedUser(Cprocess *process, u32 flags) {
    u32 indexTableDir, indexTablePage;
    u32* phisicalAddressTablePagePtr;
    u32* dirPagesKernel = (u32*) VM_KERNEL_PAGE_DIR;
    u8* virtualAdd = (u8*)vAddresStart;

    for (int i = 0; i < size; i++) {
        indexTableDir = ((u32) (virtualAdd)) >> 22;
        phisicalAddressTablePagePtr = (u32*) ((dirPagesKernel[indexTableDir] & 0xFFFFF000));
        indexTablePage = (((u32) virtualAdd & 0x003FF000) >> 12);
        x86.virtualMemory.insertPageToPageDirectoryUser(process, virtualAdd, (u8*) phisicalAddressTablePagePtr[indexTablePage]);
        virtualAdd += VM_PAGE_SIZE;
    }
    mappedUser = true;
}

void CmemoryRegion::unmapedKernel() {
    u8* virtualAdd = (u8*) vAddresStart;
    for (int i = 0; i < size; i++) {
        x86.virtualMemory.releasePagesHeapToKernel(virtualAdd);
        virtualAdd += VM_PAGE_SIZE;
    }
    mappedKernel = false;
}

CmemoryRegion::~CmemoryRegion() {
}