#ifndef SYSCALL_H
#define SYSCALL_H
#define u32 unsigned int

//Lamada a los modulos del sistema
enum eSystemCall {
    sysConsole = 1, sysTime = 2,
    sysExclusion = 3, sysSystem = 4, sysFlushDisk = 5,
    sysGraphics = 6, sysMouse = 7, sysAdminProcess = 8,
    sysFileSystem = 9, sysKeyBoard = 10, sysSharedMemory = 11
};

u32 sysCall_1(u32 anumber, u32 parameter1);
u32 sysCall_2(u32 anumber, u32 parameter1, u32 parameter2);
u32 sysCall_3(u32 anumber, u32 parameter1, u32 parameter2, u32 parameter3);
u32 sysCall_4(u32 anumber, u32 parameter1, u32 parameter2, u32 parameter3, u32 parameter4);
u32 sysCall_5(u32 anumber, u32 parameter1, u32 parameter2, u32 parameter3, u32 parameter4, u32 parameter5);

#endif

