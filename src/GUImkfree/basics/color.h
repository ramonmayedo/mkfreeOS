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

#ifndef COLOR_H
#define COLOR_H

class Gcolor {
public:
    Gcolor();
    Gcolor(int color);
    Gcolor(char r,char g, char b);
    void setcolor(int color);
    int valColor();
    char red();
    void setred(char r);
    char green();
    void setgreen(char g);
    char blue();
    void setblue(char b);
    char alpha();
    void setalpha(char a);
    Gcolor widthAlpha(char a);
    Gcolor grayScale();
    Gcolor darkned();
    Gcolor lightned();
    Gcolor inverted();
    bool operator == (Gcolor &color);
    bool operator != (Gcolor &color);
private:
    int  vcolor;
};

#endif

