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

#ifndef CSTRING_H
#define CSTRING_H
#include "vstring.h"

class string {
public:
    string();
    string(char *strchar);
    string(char *strchar, int count);
    string(const char onlyChar);
    string(const string &str);
    string(const int integer);
    string & operator=(const string &otherstr);
    string & operator=(char *strchar);
    string & operator+=(char *strchar);
    string & operator+=(string & otherstr);
    string & operator+=(const int integer);
    string & operator+(string & otherstr);
    char operator [](int index);
    bool operator==(string & otherstr);
    bool operator==(char *strchar);  
    char *c_str();
    int insertChar(int index, char nchar);
    int deleteChar(int index);
    int deleteStrChar(int index, int count);
    int insertStringChar(int index, char *str, int count);
    int insertString(string *str);
    int Lenght();
    ~string();
private:
    Vstring *data;
};

#endif

