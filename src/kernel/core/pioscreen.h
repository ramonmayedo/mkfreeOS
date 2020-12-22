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

#ifndef PIOSCREEN_H
#define PIOSCREEN_H
#include "../architecture/x86/ioscreen.h"
#include "../defines/enums.h"
#include "includes/hutils.h"

#define CAR_JMPLINE       0x0A
#define CAR_RETCURSOR     0x0D
#define CAR_ATTRIBUTE     0x07
#define CAR_BACKSPACE     0x08

class Cpioscreen {
public:
    Cpioscreen();
    void connectScreen();
    int connectProcess(Cprocess *aprocess);
    void saveScreen();
    void restoreScreen();
    int putCharOut(char car);
    char getChar();
    int command(int acommand, int parameter1, int parameter2);
    int lock();
    int unlock();
    int setKeyPress(char akey);

private:
    CioScreen *ioscreen;
    Clist *locksProcess;
    char *bufferIn;
    char *bufferOut;
    int curOut;
    int countOut;
    int curBlock;
    int state;
};

#endif

