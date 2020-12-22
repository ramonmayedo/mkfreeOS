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

#ifndef PIC8259_H
#define PIC8259_H

#include "../../defines/types.h"

#define PC_MASTER_CONTROL_REGISTER 0x21
#define PC_MASTER_BASE_REGISTER    0x20

#define PC_SLAVE_CONTROL_REGISTER 0xA1
#define PC_SLAVE_BASE_REGISTER    0xA0

class Cpic8259 {
public:
    Cpic8259();
    void initialize();
    void setICW(u32 aicw,u32 aport);
    void diseabled();
    void eoi();
private:

};

#endif

