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

#include "cstring.h"
#include "../../library/includes/conversion.h"
#include "../../library/includes/string.h"

string::string() {
    data = new Vstring(1, char(0x0));
}

string::string(char* strchar) {
    int sizeStr = strlen(strchar);
    data = new Vstring(sizeStr, strchar);
    data->insertLast(char(0x0));
}

string::string(char* strchar, int count) {
    data = new Vstring(count, strchar);
    data->insertLast(char(0x0));
}

string::string(const char onlyChar) {
    data = new Vstring(1, onlyChar);
    data->insertLast(char(0x0));
}

string::string(const string &str) {
    data = new Vstring(*str.data);
}

string::string(const int integer) {
    data = new Vstring(1);
    data->setSize(IntToStrChar(integer, data->data()));
}

string& string::operator=(char* strchar) {
    data->clear();
    data->append(strchar, strlen(strchar));
    data->insertLast(char(0x0));
    return *this;
}

string& string::operator=(const string& otherstr) {
    data->clear();
    data->append(*otherstr.data);
    return *this;
}

string& string::operator+=(char* strchar) {
    data->removeLast();
    data->append(strchar, strlen(strchar));
    char end = 0x0;
    data->insertLast(end);
    return *this;
}

string& string::operator+=(string& otherstr) {
    data->removeLast();
    data->append(*otherstr.data);
    return *this;
}

string & string::operator+=(const int integer) {
    string newStr = integer;
    this->operator +=(newStr);
    return *this;
}

string& string::operator+(string& otherstr) {
    return operator +=(otherstr);
}

char string::operator[](int index) {
    if (data->count() > index) {
        return data->operator[](index);
    }
    return 0x0;
}

bool string::operator==(string& otherstr) {
    return !strcmp(c_str(), otherstr.c_str());
}

bool string::operator==(char* strchar) {
    return !strcmp(c_str(), strchar);
}

int string::Lenght() {
    return data->count() - 1;
}

char* string::c_str() {
    return data->data();
}

int string::insertChar(int index, char nchar) {
    data->insert(index, nchar);
}

int string::deleteChar(int index) {
    data->remove(index);
}

int string::deleteStrChar(int index, int count) {
    data->remove(index, count);
}

int string::insertStringChar(int index, char* str, int count) {
    data->insert(index, str, count);
}

int string::insertString(string *str) {
    data->removeLast();
    data->append(*(str->data));
}

string::~string() {
    data->clear();
    delete(data);
}