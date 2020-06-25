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

#include "adminthread.h"
#include "../architecture/x86/x86.h"
extern Sx86 x86;

CadminThread::CadminThread() {
    run = 0;
    tid = 0;
}

int CadminThread::addReady(SregX86 *regX86) {
    tid++;
    Cthread *thread = new Cthread(regX86, tid);
    ready.add(thread);           
    return tid;
}

SregX86 * CadminThread::schelude() {
    if (ready.count() > 0) {
        Cthread* read = (Cthread*) ready.removeFirst();
        if (run != 0) ready.add(run);
        run = read;
    }
    return run->getregX86();
}

Cthread *CadminThread::getRun() {
    return run;
}

CadminThread::~CadminThread() {
    delete(run);
    deleteList(&ready);
    deleteList(&lock);
}

void CadminThread::deleteList(Clist *list) {
    Cthread *thread;
    for (int i = 0; i < list->count(); i++) {
        thread = (Cthread*) list->getItem(i);
        delete(thread);
    }
    list->clear();
}

int CadminThread::killThread(int atid) {
    Cthread *thread;
    if (atid == 1) return 0;
    if (run->gettid() == atid) {
        delete(run);
        run = 0;
        return atid;
    }
    for (int i = 0; i < ready.count(); i++) {
        thread = (Cthread*) ready.getItem(i);
        if (thread->gettid() == atid) {
            ready.delet(i);
            delete(thread);
            return atid;
        }
    }
    return 0;
}

int CadminThread::sendState(int state) {
    run->sendState(state);
    if (state > 0) {
        lock.add(run); //Se define como bloqueado
        run = 0;
    }
    return ready.count();
}

int CadminThread::unlockThread(Cthread *thread) {
    thread->sendState(0);
    lock.remove(thread);
    ready.add(thread);
    return thread->gettid();
}

Cthread* CadminThread::sendSignal(int signal) {
    Cthread *thread;
    for (int i = 0; i < lock.count(); i++) {
        thread = (Cthread*) lock.getItem(i);
         //x86.ioScreen.printf("sig = %i",thread->waitSignal);
        if (thread->waitSignal == signal) {
            thread->waitSignal = 0;                  
            return thread;
        }
    }
    return 0;
}
