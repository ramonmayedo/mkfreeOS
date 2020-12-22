/*Copyright (C) 2019  Ram�n Mayedo Morales (ramonmayedo@gmail.com)

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

#include "../includes/conio.h"
#include "../includes/syscall.h"

enum commandConsole {
    cmcColor = 1, cmcGotoXY = 2, cmcWhereX = 3, cmcWhereY = 4,
    cmcPrintf = 5, cmcLock = 6, cmcUnlock = 7, cmcGetChar = 8,
    cmcPutChar = 9, cmcReadChar = 10, cmcWrite = 11
};

enum commandGraphics {
    cmgChangeModeVideo = 1, cmgPaintArea = 2, cgpChangeModeVideoAdvanced = 3,
    cmpGetArea = 4
};

enum commandMouse {
    cmmMouseEvent = 1
};

enum eCommandKeyboard {
    cmkKeyEvent = 1
};


void textcolor(int newcolor) {
    sysCall_2(sysConsole, cmcColor, newcolor);
}

void gotoxy(int posX, int posY) {
    sysCall_3(sysConsole, cmcGotoXY, posX, posY);
}

int wherex() {
    return sysCall_1(sysConsole, cmcWhereX);
}

int wherey() {
    return sysCall_1(sysConsole, cmcWhereY);
}

int blockDisplay() {
    return sysCall_1(sysConsole, cmcLock);
}

int unblockDisplay() {
    return sysCall_1(sysConsole, cmcUnlock);
}

int changeMode(int mode) {
    return sysCall_2(sysGraphics, cmgChangeModeVideo, mode);
}

int paintArea(SvideoArea *area) {
    return sysCall_2(sysGraphics, cmgPaintArea, (int) area);
}

int getMouseState(int *dx, int *dy, int *dz) {
    return sysCall_4(sysMouse, cmmMouseEvent, (int) dx, (int) dy, (int) dz);
}

int getKeyPress(){
    return sysCall_1(sysKeyBoard, cmkKeyEvent); 
}

int changeModeAdvance(int mode,int width,int height){
    return sysCall_3(sysGraphics, cgpChangeModeVideoAdvanced, (int) width, (int) height);
}

int getArea(SvideoArea *area) {
    return sysCall_2(sysGraphics, cmpGetArea, (int) area);
}