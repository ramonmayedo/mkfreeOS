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

#include "thread.h"
#include "../architecture/x86/x86.h"
#include "adminProcess.h"
extern Sx86 x86;
extern Score core;

Cthread::Cthread(SregX86 *aregX86, Cprocess *parent, bool ismainThread) {
    process = parent;
    regX86 = aregX86;
    state = 0;
    waitSignal = 0;
    mainThread = ismainThread;
    controlTime.filter = 0;
    controlTime.plannedTime = process->priority;
    controlTime.standarTime = process->priority;
    controlTime.beforeConsumedTime = process->priority;
    controlTime.timeConsumed = 0;
}

void Cthread::setmainThread() {
    mainThread = true;
}

bool Cthread::getmainThred() {
    return mainThread;
}

Cthread::~Cthread() {
    delete(regX86);
}

int Cthread::gettid() {
    return tid;
}

void Cthread::settid(int atid) {
    tid = atid;
}

int Cthread::sendState(int astate, u32 filtro) {
    if (astate > 0) {
        regX86->eip -= 2;
        state = THREAD_BLOCK;
        controlTime.filter = filtro;
        core.adminProcess.schelude();
    }
    return 0;
}

void Cthread::onActivate() {
    state = THREAD_RUN;
    controlTime.timeConsumed += controlTime.beforeConsumedTime;
    controlTime.plannedTime = ((controlTime.filter * controlTime.beforeConsumedTime + (100 - controlTime.filter) * controlTime.standarTime) / 100);
    controlTime.beforeConsumedTime = 0;
}