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

#include "../includes/stdlib.h"
#include "../includes/syscall.h"
enum commandSytem{
    cmsReset = 1, cmsShutDown = 2
};

int atoi(char *str) {
    int sig = 1, n = 0, res = 0;
    if (str == 0) return 0;
    
    if (str[0] == '-') {
        sig = -1;
        n++;
    }

    while (str[n] != 0x0) {
        res = res * 10 + (str[n] - '0');
        n++;
    }

    return (res*sig);
}

int reset(){
    return sysCall_1(sysSystem,cmsReset);
}

int shutDown() {
    return sysCall_1(sysSystem, cmsShutDown);
}