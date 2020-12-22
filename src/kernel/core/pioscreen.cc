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

#include "pioscreen.h"
#include "../architecture/x86/x86.h"

extern Score core;
extern Sx86 x86;

Cpioscreen::Cpioscreen() {

}

void Cpioscreen::connectScreen() {
    ioscreen = &x86.ioScreen;
    countOut = 0;
    curBlock = 0;
    curOut = 0;
    state = 0;
    bufferIn = (char*) x86.virtualMemory.getPagesHeapToKernel();
    bufferOut = (char*) x86.virtualMemory.getPagesHeapToKernel();
    locksProcess = new Clist();
}

int Cpioscreen::putCharOut(char car) {
    if (car == CAR_BACKSPACE) {
        if (countOut > 0){
            countOut--;
        }
        return 0;
    }
    bufferIn[countOut] = car;
    return countOut++;
}

char Cpioscreen::getChar() {
    char car = bufferIn[curOut];
    curOut++;
    countOut--;
    return car;
}

int Cpioscreen::command(int acommand, int parameter1, int parameter2) {
    switch (acommand) {
        case cmcReadChar:
        {
            if (countOut == 0) {
                curOut = 0;
                curBlock = ioscreen->getX();
                return 0;
            }
            return (int) getChar();
        }
        case cmcGetChar:
        {
            int key =  core.keyborad.command(cmkKeyEvent, parameter1, parameter2);
            setKeyPress(key);
            return key;
        }
        case cmcLock:
        {
            if (state == cmdScreeLock) //Si esta bloqueado ya !!!
                lock();                //Se bloque el proceso que intenta bloquerlo
            state = cmdScreeLock;      //Si no se bloquea la pantalla
            return 0;
        }
        case cmcUnlock:
        {
            unlock();
            return 0;
        }
        case cmcColor:
        {
            ioscreen->setTextColor(parameter1);
            return 0;
        }
        case cmcGotoXY:
        {
            ioscreen->setX(parameter1);
            ioscreen->setY(parameter2);
            return 0;
        }
        case cmcWhereX:
        {
            return ioscreen->getX();
        }
        case cmcWhereY:
        {
            return ioscreen->getY();
        }
        case cmcPrintf:
        {
            ioscreen->printk((char*) parameter1, parameter2);
            return 0;
        }
        case cmcPutChar:
        {
            ioscreen->putC((char) parameter1);
            return 0;
        }
        case cmcWrite:
        {
            char *ptrCar = (char*) parameter1;
            for (int i = 0; i < parameter2; i++)
                ioscreen->putC(ptrCar[i]);
            return 0;
        }
        default: return 0;
    }
}

int Cpioscreen::setKeyPress(char akey) {
    if (akey == CAR_JMPLINE) curBlock = 0;
    int curX = ioscreen->getX();
    if (curBlock < curX || (curBlock == curX && akey != CAR_BACKSPACE))
        ioscreen->putC(akey);
    putCharOut(akey);
}

int Cpioscreen::lock() {
    SlockProcess *lockProcess = new SlockProcess;
    lockProcess->thread = core.adminProcess.getRun();
    lockProcess->state = cmdScreeLock;
    locksProcess->add(lockProcess);
    lockProcess->thread->sendState(cmdScreeSleep,60); //se suspende     
}

int Cpioscreen::unlock() {
    while (locksProcess->count()) { //Si hay procesos bloqueados
        SlockProcess *lockProcess = (SlockProcess*) locksProcess->removeFirst(); //Tomo el primero de la lista
        if (core.adminProcess.unlockThread(lockProcess->thread, ADP_PRIORITY_LOW) != -1)
            break;
    }
    state = cmdUnlock; //Se desbloquea
}