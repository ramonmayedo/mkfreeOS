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
#include "thread.h"

#define THREAD_RUN   0x0
#define THREAD_READY 0x1
#define THREAD_BLOCK 0x2

#define ADP_PRIORITY_LOW   0x0
#define ADP_PRIORITY_HIGHT 0x1

enum commandAdminProcess {
    cmpExecV = 1, cmpExit = 2, cmpCreateThread = 3, cmpKillThread = 4, cmpSrbk = 5,
    cmpSendCommand = 6, cmpGetCommand = 7, cmpGetPid = 8,
    cmpReadBufferIPC = 10, cmpWriteBufferIPC = 11, cmpSharedMemory = 12
};

enum priorityAdminProcess {
    priorityVeryHight = 50, priorityLow = 15, priorityHight = 40, priorityVeryLow = 1,
    priorityNormal = 25
};

class CadminProcess {
public:
    CadminProcess();
    void initialize();
    int addReady(char *afile, int argc, char **argv, int priority);
    int addThread(Cthread *thread);
    void schelude();
    Cthread *getRun();
    Cthread *getNext();
    int deleteThread(int tid);
    void killProcessRun();
    int unlockThread(Cthread *thread, int priority);
    int isThreadExist(Cthread *thread);
    Cprocess *getProcess(int pid);
    Cthread *getThread(int tid);
    Cthread* sendSignal(int signal, Cprocess *process);
    int command(int acommand, int parameter1, int parameter2, int parameter3, int parameter4);
    void tick(u32 *ebp);
private:
    Clist *ready, *lock, *processList;
    Cthread *run;
    int max, pid, tid;
    void deleteProcess(Cprocess *process);

};

#endif

