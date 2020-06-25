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

#include "systemcall.h"
#include "../architecture/x86/x86.h"

extern Sx86 x86;
extern Score core;
extern Smaps maps;

CsystemCall::CsystemCall() {

}

u32 CsystemCall::systemCall(u32 anumber, u32 parameter1, u32 parameter2, u32 parameter3, u32 parameter4, u32 parameter5) {

    switch (anumber) {
        case sysFileSystem:
        {
            return core.fileSystem.command(parameter1, parameter2, parameter3, parameter4, parameter5);
        }
        case sysConsole:
        {
            return core.pioscreen.command(parameter1, parameter2, parameter3);
        }
        case sysTime:
        {
            return core.clock.command(parameter1, parameter2, parameter3);
        }
        case sysExclusion:
        {
            return core.exclusion.command(parameter1, parameter2, parameter3);
        }
        case sysSystem:
        {
            if (parameter1 == cmsReset) {
                core.cacheDisk.flush();
                x86.architecture.reset();
            }else if (parameter1 == cmsShutDown){
                core.cacheDisk.flush();
                x86.architecture.shutDown();
            }
            return 0;
        }
        case sysAdminProcess:
        {
            return core.adminProcess.command(parameter1, parameter2, parameter3, parameter4, parameter5);
        }
        case sysGraphics:
        {
            return core.graphics.command(parameter1, parameter2, parameter3);
        }
        case sysMouse:
        {
            return core.mousePS2.command(parameter1, parameter2, parameter3);
        }
        case sysKeyBoard:
        {
            return core.keyborad.command(parameter1, parameter2, parameter3);
        }
        case sysSharedMemory:
        {
            return x86.sharedMemory.command(parameter1, parameter2, parameter3, parameter4);
        }
        default:return 0;
    }
}