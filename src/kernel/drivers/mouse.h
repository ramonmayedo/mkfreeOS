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

#ifndef MAUSE_H
#define MAUSE_H
#include "../defines/types.h"
#include "../core/process.h"
#include "../core/thread.h"
#include "../core/includes/hutils.h"

#define MS_CONTROL_REGISTER 0x64
#define MS_BASE_REGISTER    0x60

struct SmouseMap {
    int dx;
    int dy;
    int dz;
    int state;
    u8 flags;
    u8 buttons;
} __attribute__((packed));

class CmausePS2 {
public:
    CmausePS2();
    void initialize();
    void IRQ();
    int command(int acommand, int parameter1, int parameter2);
    int getState();
private:
    u32 state;
    Clist locksProcess;
    SlockProcess *lockProcess;
    char buffer[32];
    u8 ptr;
    u8 count;
    SmouseMap mouseMap ;
    u8 waitRead(u32 port);
    void waitWrite(u32 port, u8 value);
    void waitOutPut();
    void waitInPut();
    void write(u8 data);
    u8 read();
    void ack();
    int lock();
    int unlock();
    int confirmUnlock();
    
};

#endif

