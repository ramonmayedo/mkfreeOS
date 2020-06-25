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

#include "../includes/syscall.h"
#include "../includes/unistd.h"

enum commandAdminProcess {
    cmpExecV = 1, cmpExit = 2, cmpCreateThread = 3, cmpKillThread = 4, cmpSrbk = 5,
    cmpSendCommand = 6, cmpGetCommand = 7, cmpGetPid = 8, cmpWaitCommand = 9,
    cmpReadBufferIPC = 10, cmpWriteBufferIPC = 11
};

enum commandExclusion {
    cmeNewSemaphore = 1, cmeDelSemaphore = 2,
    cmeBlockSemaphore = 3, cmeUnblockSemaphore = 4
};

enum commandSharedMemory{
    cmsGetZone = 1, cmsAddZone = 2, cmsSharedZone = 3
};

void* sbrk(int incSize) {
    return (void*) sysCall_2(sysAdminProcess, cmpSrbk, incSize);
}

void exit(int astatus) {
    sysCall_2(sysAdminProcess, cmpExit, astatus);
}

int execv(const char* fileName, char* const argv[]) {
    return sysCall_3(sysAdminProcess, cmpExecV, (int) fileName, (int) argv);
}

int newSemaphore() {
    return sysCall_1(sysExclusion, cmeNewSemaphore);
}

int delSemaphore(int sid) {
    return sysCall_2(sysExclusion, cmeDelSemaphore, sid);
}

int blockSemaphore(int sid) {
    return sysCall_2(sysExclusion, cmeBlockSemaphore, sid);
}

int unblockSemaphore(int sid) {
    return sysCall_2(sysExclusion, cmeUnblockSemaphore, sid);
}

void diskflush(){
    sysCall_1(sysFlushDisk,0);
}

int getCommand(int *command, int *pidTrasmisor, int *parameter1, int *parameter2) {
    return sysCall_5(sysAdminProcess, cmpGetCommand, (int) command, (int) pidTrasmisor, (int) parameter1, (int) parameter2);
}

int sendCommand(int command, int pidReceptor, int parameter1, int parameter2) {
    return sysCall_5(sysAdminProcess, cmpSendCommand, command, pidReceptor, parameter1, parameter2);
}

int getPid() {
    return sysCall_1(sysAdminProcess, cmpGetPid);
}

int waitCommand(int command, int *pidTrasmisor, int *parameter1, int *parameter2){
    return sysCall_5(sysAdminProcess, cmpWaitCommand, (int) command, (int) pidTrasmisor, (int) parameter1, (int) parameter2);
}

int readBufferIPC(int pid, char *buffer, int offset, int size) {
    return sysCall_5(sysAdminProcess, cmpReadBufferIPC, pid, (int) buffer, offset, size);
}

int writeBufferIPC(int pid, char *buffer, int offset, int size) {
    return sysCall_5(sysAdminProcess, cmpWriteBufferIPC, pid, (int) buffer, offset, size);
}

int shmem(int size){
    return sysCall_2(sysSharedMemory, cmsSharedZone, size);
}

int setshmem(int pid, int sid, int size){
    return sysCall_4(sysSharedMemory, cmsAddZone, pid, sid, size);
}