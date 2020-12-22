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
#include "process.h"

class Cthread {
public:
    Cthread(SregX86 *aregX86, Cprocess *parent,bool ismainThread);
    int sendState(int astate, u32 filtro);
    SregX86 *regX86;
    int waitSignal;
    int state;
    ScontrolTime controlTime;
    Cprocess *process;
    int gettid();
    void settid(int atid);
    void setmainThread();
    bool getmainThred();
    void onActivate();
    ~Cthread();
private:
    int tid;
    
    bool mainThread;
};

#endif

