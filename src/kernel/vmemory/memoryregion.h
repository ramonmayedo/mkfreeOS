#ifndef MEMORYREGION_H
#define MEMORYREGION_H

#include "../defines/types.h"
#include "../core/process.h"

class CmemoryRegion {
public:
    CmemoryRegion(u32 avAddresStart, u32 apAddresStart, u32 asize, bool aonlyKernel);
    void setShared(bool shared);
    void setMappedKernel(u32 flags);
    void setMappedKernelPhisV(u32 flags);
    void unmapedKernel();
    void setMappedUser(Cprocess *process,u32 flags);
    int key;
    u32 vAddresStart, pAddresStart, size;
    bool isShared;
    ~CmemoryRegion();
private:
    bool onlyKernel, mappedKernel, mappedUser;
    
};

#endif

