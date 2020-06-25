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

#ifndef THREAD_H
#define THREAD_H
#include "../architecture/x86/includes/reg.h"

class Cthread {
public:
    Cthread(SregX86 *aregX86, int atid);
    void setregX86(SregX86 *aregX86);
    void sendState(int astate);
    SregX86 *getregX86();
    int waitSignal;
    int getState();
    int gettid();
    ~Cthread();
private:
    SregX86 *regX86;
    int tid;
    int state;
};

#endif

