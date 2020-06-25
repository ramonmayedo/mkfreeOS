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

#ifndef PROCESS_H
#define PROCESS_H
#include "../defines/types.h"
#include "includes/hprocess.h"
#include "../uses/list.h"
#include "adminthread.h"

class Cprocess {
public:
    Cprocess(u8 *afile, int argc, char **argv, int apid);
    int addFile(void* afile);
    void *getFile(int aidFile);
    int deleteFile(int aidFile);
    int getpid();
    void setParent(Cprocess *aparent);
    Cprocess *getParent();
    int addProcessChild(Cprocess *achild);
    int deleteProcessChild(Cprocess *achild);
    int sendState(int acommand);
    char *getbufferIPC();
    int getcurrentState();
    void onActivate();
    void onDesactivate();
    SprocessX86 processX86;
    CadminThread *adminThread; 
    int  sharedMemory;
    int addThread(u32 eip);
    int getCommand(int command,int parameter1,int parameter2, int parameter3);
    int sendCommad(int command,int parameter1,int parameter2, int aprameter3);
    int getWaitCommand(int command,int parameter1,int parameter2, int aprameter3);
    ~Cprocess();

private:
    u32 pid;
    Clist files;
    Clist processChild;
    Cprocess *processParent;
    u32 sizeMemory;
    int countFiles;
    int type;
    int currentState;
    char *bufferIPC;
   SCommandProcess *commands;
protected:
    char** saveArgMainToKernel(int argc, char **argv);
    u8* saveArgMain(int argc, char **argv, char *stackPtr);
    void deleteAllFiles();
    int deleteAllProcessChild();
};

#endif

