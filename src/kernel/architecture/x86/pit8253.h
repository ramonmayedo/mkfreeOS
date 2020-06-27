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

#ifndef PIT8253_H
#define PIT8253_H

#define TICKS_PER_SECOND 1000

#define TIMER0_CTL 0x0
#define TIMER1_CTL 0x1
#define TIMER2_CTL 0x2

#define TIMER0_SELECT 0x00
#define TIMER1_SELECT 0x40
#define TIMER2_SELECT 0x80

#define MODE_COUNTDOWN 0x00
#define MODE_ONESHOT 0x02
#define MODE_RATE 0x04
#define MODE_SQUARE_WAVE 0x06

#define WRITE_WORD 0x30

#define BASE_FREQUENCY 1193182


class Cpit8253 {
public:
    Cpit8253();
    void initialize();
    ~Cpit8253();
private:

};

#endif

