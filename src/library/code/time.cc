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

#include "../includes/time.h"
#include "../includes/syscall.h"

enum eCommandClock {
    cmtGetTimeU32 = 1
};

int time(int* timer) {
    int time;
    time = sysCall_2(sysTime, cmtGetTimeU32, (u32) & time);
    if (timer != 0)
        *timer = time;
    return time;
}

void gmtime(const int timer, stime *time) {
    time->hour = timer / 3600000;
    time->min = (timer % 3600000) / 60000;
    time->sec = ((timer % 3600000) % 60);
    time->msec = timer % 1000;
}