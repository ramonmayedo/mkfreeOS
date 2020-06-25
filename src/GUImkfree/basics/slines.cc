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

#include "slines.h"

Gslines::Gslines() {
    lines = new Clist();
}

void Gslines::insert(int index, string* str) {
    if (index >= count())
        lines->add(str);
    else
        lines->insert(index, str);
}

void Gslines::add(string* str) {
    lines->add(str);
}

string* Gslines::operator[](int index) {
    return (string*) lines->getItem(index);
}

void Gslines::Delete(int index) {
    string *deleteString = (string*) lines->getItem(index);
    lines->delet(index);
    if (deleteString) delete(deleteString);
}

void Gslines::clear() {
    for (int i = 0; i < lines->count(); i++)
        delete((string*) lines->getItem(i));
    lines->clear();
}

int Gslines::count() {
    return lines->count();
}

Gslines::~Gslines() {   
}

