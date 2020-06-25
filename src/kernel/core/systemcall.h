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

#ifndef SYSTEMCALL_H
#define SYSTEMCALL_H
#include "../defines/types.h"

enum eSystemCall {
    sysConsole = 1, sysTime = 2,
    sysExclusion = 3, sysSystem = 4, sysFlushDisk = 5,
    sysGraphics = 6, sysMouse = 7, sysAdminProcess = 8,
    sysFileSystem = 9, sysKeyBoard = 10, sysSharedMemory = 11
};

enum eCommandSytem{
    cmsReset = 1, cmsShutDown = 2
};

class CsystemCall {
public:
    CsystemCall();
    u32 systemCall(u32 anumber, u32 parameter1, u32 parameter2, u32  parameter3, u32 parameter4, u32 parameter5);
private:

};

#endif

