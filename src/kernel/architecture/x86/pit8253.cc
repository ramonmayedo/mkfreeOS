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

#include "pit8253.h"
#include "x86.h"

extern Sx86 x86;
extern Smaps maps;

Cpit8253::Cpit8253() {
}

void Cpit8253::initialize() {
    maps.ticks = 0;
    x86.port.outb(PT_CONTROL_REGISTER, TIMER0_SELECT | WRITE_WORD | MODE_SQUARE_WAVE);
    u16 timerReload = (BASE_FREQUENCY / TICKS_PER_SECOND);
    x86.port.outb(PT_BASE_REGISTER + TIMER0_CTL, timerReload);
    x86.port.outb(PT_BASE_REGISTER + TIMER0_CTL, timerReload >> 8);
}

void Cpit8253::waitms(u32 msec) {

}

Cpit8253::~Cpit8253() {
}

