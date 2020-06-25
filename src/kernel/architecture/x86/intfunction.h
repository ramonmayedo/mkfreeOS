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

#ifndef INTFUNCTION_H
#define INTFUNCTION_H

extern "C"{
    void  c_passint();
    void  c_int_timer();
    void  c_int_keyboard();
    void  c_int_disk0();
    void  c_gp();
    void  c_page_fault();
    void  c_syscall();
    void  c_syscall_ret();
    void  c_scheluder();
    void  c_mouse_ps2();
}

#endif

