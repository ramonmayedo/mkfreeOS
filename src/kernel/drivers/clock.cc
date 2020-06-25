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

#include "clock.h"
#include "../architecture/x86/x86.h"

extern Smaps maps;
extern Sx86 x86;

Cclock::Cclock() {
}

void Cclock::getTime(u32 *ahour, u32 *aminute, u32 *asecond) {
    u32 dataHour, dataMinute, dataSecond;
    x86.port.outb(maps.clockMap.port.regBase, 0x95);

    x86.port.outb(maps.clockMap.port.regBase, 4);
    dataHour = x86.port.inb(maps.clockMap.port.regControl);
    *ahour = dataHour - ((u32) dataHour / 16) * 6;

    x86.port.outb(maps.clockMap.port.regBase, 2);
    dataMinute = x86.port.inb(maps.clockMap.port.regControl);
    *aminute = dataMinute - ((u32) dataMinute / 16) * 6;

    x86.port.outb(maps.clockMap.port.regBase, 0);
    dataSecond = x86.port.inb(maps.clockMap.port.regControl);
    *asecond = dataSecond - ((u32) dataSecond / 16) * 6;
}

void Cclock::getDate(u32 *ayear, u32 *amonth, u32 *aday) {
    u32 dataYear, dataMonth, dataDay;

    x86.port.outb(maps.clockMap.port.regBase, 0x95);

    x86.port.outb(maps.clockMap.port.regBase, 9);
    dataYear = x86.port.inb(maps.clockMap.port.regControl);
    *ayear = dataYear - ((u32) dataYear / 16) * 6;

    x86.port.outb(maps.clockMap.port.regBase, 8);
    dataMonth = x86.port.inb(maps.clockMap.port.regControl);
    *amonth = dataMonth - ((u32) dataMonth / 16) * 6;

    x86.port.outb(maps.clockMap.port.regBase, 7);
    dataDay = x86.port.inb(maps.clockMap.port.regControl);
    *aday = dataDay - ((u32) dataDay / 16) * 6;
}

void Cclock::getWeekDay(u32 *aweekDay) {
    u32 dataWeekday;

    x86.port.outb(maps.clockMap.port.regBase, 0x95);
    x86.port.outb(maps.clockMap.port.regBase, 6);
    dataWeekday = x86.port.inb(maps.clockMap.port.regControl);
    if (dataWeekday < 6) *aweekDay = dataWeekday + 2;
    else *aweekDay = dataWeekday - 5;

}

void Cclock::refresh() {
    refreshTime();
    refreshDate();
    refreshWeekDay();
}

void Cclock::refreshTime() {
    getTime(&maps.clockMap.hour, &maps.clockMap.minute, &maps.clockMap.second);
}

void Cclock::refreshDate() {
    getDate(&maps.clockMap.year, &maps.clockMap.month, &maps.clockMap.day);
}

void Cclock::refreshWeekDay() {
    getWeekDay(&maps.clockMap.weekDay);
}

u32 Cclock::getTimeU32() {
    u32 hours, minutes, seconds;
    getTime(&hours, &minutes, &seconds);
    return ((hours * 60 + minutes)*60 + seconds)* MAX_TICK_X_SECONDS + maps.clockMap.msecond;
}

int Cclock::command(int acommand, int parameter1, int parameter2) {
    switch (acommand) {
        case cmtGetTimeU32:
        {
            return getTimeU32();
        }
        default:return 0;
    }
}