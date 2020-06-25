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

#ifndef SLINES_H
#define SLINES_H
#include "cstring.h"
#include "list.h"

class Gslines {
public:
    Gslines();
    void insert(int index, string *str);
    void add(string *str);
    void Delete(int index);
    void clear();
    string *operator[](int index);
    int count();
    ~Gslines();
private:
    Clist *lines;
};

#endif