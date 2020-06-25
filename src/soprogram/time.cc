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

#include <cprogram.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>


int kmain(int argc, char** argv) {
    int mtime;
    stime ftime;
    mtime = time(0);
    gmtime(mtime, &ftime);
    printf("Son las %i:%i:%i \n", ftime.hour, ftime.min, ftime.sec);
    int *a = (int*)shmem(2048);
    unblockDisplay();
    return 0;
}
