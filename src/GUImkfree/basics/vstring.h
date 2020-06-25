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

#ifndef VSTRING_H
#define VSTRING_H

class Vstring {
public:
    Vstring(int n);
    Vstring(int n, char data);
    Vstring(int n, char *data);
    Vstring(Vstring &vector);
    char operator[](int index);
    char first();
    char last();
    char removeFirst();
    char removeLast();
    char remove(int index);
    void remove(int index, int count);
    void needCapacity(int newCapacity);
    void insert(int index, char value);
    void insert(int index, char *values, int count);
    void insertFirst(char value);
    void insertLast(char value);
    void append(char value);
    void append(char *values, int count);
    void append(Vstring &vector);
    void clear();
    int count();
    bool emty();
    char *data();
    void setSize(int newSize);
private:
    char *vstart;
    int vsize, vindex, vcapacity;
protected:
    void intialized(int n);
};

#endif

