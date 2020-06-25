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

#ifndef EXCLUSION_H
#define EXCLUSION_H
#include "../uses/list.h"


enum commandExclusion{
     cmeNewSemaphore = 1, cmeDelSemaphore = 2, 
     cmeBlockSemaphore = 3, cmeUnblockSemaphore = 4
};

class Csemaphore{
public:
    Csemaphore(int asid){sid = asid;value=0;}
    int getsid(){return sid;}
    void setValue(int avalue){value = avalue;}
    int  getValue(){return value;}
private:
    int sid;
    int value;
};


class Cexclusion {
public:
    Cexclusion();
    int up(int aid);
    int down(int aid);
    int newSemaphore();
    int delSemaphore(int aid);
    int valSemaphore(int aid,int avalue);
    int command(int acommand, int parameter1, int parameter2);
private:
    Clist semaphores;
    int   sid;
protected:
    void *getSemaphore(int asid);
};

#endif 

