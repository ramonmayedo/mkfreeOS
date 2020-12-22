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

#ifndef CLOCK_H
#define CLOCK_H
#include "../defines/types.h"

#define CK_CONTROL_REGISTER 0x71
#define CK_BASE_REGISTER    0x70

#define MAX_TICK_X_SECONDS 1000

struct SclockMap {
    u32 weakUp;
    u32 hour;
    u32 minute;
    u32 second;
    u32 year;
    u32 month;
    u32 day;
    u32 weekDay;
} __attribute__((packed));

class Cclock {
public:
    Cclock();
    void refresh();
    void getTime(u32 *ahour, u32 *aminute, u32 *asecond);
    void getDate(u32 *ayear, u32 *amonth, u32 *aday);
    void getWeekDay(u32 *aweekDay);
    void refreshTime();
    void refreshDate();
    void refreshWeekDay();
    u32 getTimeU32();
    int command(int acommand, int parameter1, int parameter2);
private:
   
};

#endif 

