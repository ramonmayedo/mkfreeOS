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

#ifndef ENUMS_H
#define ENUMS_H


enum eCommand {
    cmdUnlock = 0, cmdKeyboardLock = 1, cmdScreeLock = 1,
    cmdKeyboardSleep = 6, cmdScreeSleep = 1, cmdSemaphoreSleep = 3, cmdMouseLock = 1,
    cmdMouseSleep = 4, cmdCommandSleep = 1
};

enum eCommandConsole {
    cmcColor = 1, cmcGotoXY = 2, cmcWhereX = 3, cmcWhereY = 4,
    cmcPrintf = 5, cmcLock = 6, cmcUnlock = 7, cmcGetChar = 8,
    cmcPutChar = 9, cmcReadChar = 10, cmcWrite = 11, cmcMouseEvent = 12
};

enum eCommandMouse {
    cmmMouseEvent = 1
};

enum eCommandKeyboard {
    cmkKeyEvent = 1
};

enum eCommandClock {
    cmtGetTimeU32 = 1
};


#endif