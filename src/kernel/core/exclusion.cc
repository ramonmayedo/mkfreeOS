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

#include "exclusion.h"
#include "../architecture/x86/x86.h"
#include "../defines/enums.h"
extern Score core;

Cexclusion::Cexclusion() {
    sid=10;
}

int Cexclusion::newSemaphore() {
    sid++;
    Csemaphore *semaphore = new Csemaphore(sid);
    semaphores.add(semaphore);
    return sid;
}

int Cexclusion::delSemaphore(int aid) {
    Csemaphore * semaphore = (Csemaphore*)getSemaphore(aid);
    if (semaphore) {
        semaphores.remove(semaphore);
        delete(semaphore);
        return aid;
    }
}

int Cexclusion::valSemaphore(int aid, int avalue) {
    Csemaphore * semaphore = (Csemaphore*) getSemaphore(aid);
    if (semaphore) {
        semaphore->setValue(avalue);
        return aid;
    }
    return 0;
}

int Cexclusion::command(int acommand, int parameter1, int parameter2) {
    switch (acommand) {
        case cmeNewSemaphore:
        {
            return newSemaphore();
        }
        case cmeDelSemaphore:
        {
            return delSemaphore(parameter1);
        }
        case cmeBlockSemaphore:
        {
            return valSemaphore(parameter1, true);
        }
        case cmeUnblockSemaphore:
        {
            return valSemaphore(parameter1, false);
        }
        default: return 0;
    }
}

void *Cexclusion::getSemaphore(int asid) {
    Csemaphore * semaphore;
    for (int i = 0; i < semaphores.count(); i++) {
        semaphore = (Csemaphore*) semaphores.getItem(i);
        if (semaphore->getsid() == asid) return (void*) semaphore;
    }
    return 0;
}

int Cexclusion::up(int aid) {
    Csemaphore * semaphore = (Csemaphore*) getSemaphore(aid);
    if (semaphore) {
        if (semaphore->getValue() == 0) semaphore->setValue(true);
        else core.adminProcess.getRun()->sendState(semaphore->getsid(),5); //Proceso Bloquedo;
        return aid;
    }
    return 0;
}

int Cexclusion::down(int aid) {
    Csemaphore * semaphore = (Csemaphore*) getSemaphore(aid);
    if (semaphore) {
        semaphore->setValue(false);
        return aid;
    }
    return 0;
}