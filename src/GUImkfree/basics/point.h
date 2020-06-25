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

#ifndef POINT_H
#define POINT_H

class Gpoint {
public:
    Gpoint();
    Gpoint(int x, int y);
    int x();
    int y();
    void setx(int x);
    void sety(int y);
    void move(int dx,int dy);
    void move(Gpoint &dpoint);
    Gpoint traslate(int dx,int dy);
    Gpoint traslate(Gpoint &point);
    bool operator == (Gpoint &point);
    Gpoint operator-();
    Gpoint operator-(Gpoint &point);
    Gpoint operator+(Gpoint &point);
    Gpoint &operator+=(Gpoint &point);
    Gpoint &operator-=(Gpoint &point);
private:
    int px, py;
};

#endif

