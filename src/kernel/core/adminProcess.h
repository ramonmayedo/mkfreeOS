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

#ifndef ADMINPROCESS_H
#define ADMINPROCESS_H
#include "../uses/list.h"
#include "process.h"

#define PROC_RUN   0
#define PROC_READY 1
#define PROC_BLOCK 2

enum commandAdminProcess {
    cmpExecV = 1, cmpExit = 2, cmpCreateThread = 3, cmpKillThread = 4, cmpSrbk = 5,
    cmpSendCommand = 6, cmpGetCommand = 7, cmpGetPid = 8, cmpWaitCommand = 9,
    cmpReadBufferIPC = 10, cmpWriteBufferIPC = 11, cmpSharedMemory = 12
};

class CadminProcess {

public:
    CadminProcess();
    int addReady(char *afile, int argc, char **argv);
    void schelude();
    Cprocess *getRun();
    Cprocess *getNext();
    int deleteProcess(int aidPID);
    void killProcessRun();
    int unlockProcess(int astate);
    int unlockProcess(Cprocess *process, Cthread *thread);
    Cprocess *getProcess(int aidPID);
    int command(int acommand, int parameter1, int parameter2, int parameter3, int parameter4);
private:
    Clist ready, lock;
    Cprocess *run;
    int max,id;
};

#endif

