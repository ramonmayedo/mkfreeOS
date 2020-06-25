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

#include "point.h"

Gpoint::Gpoint() {
    px=0;
    py=0;
}
Gpoint::Gpoint(int x, int y){
    px = x;
    py = y;
}

int Gpoint::x(){
    return px;
}

int Gpoint::y(){
    return py;
}

void Gpoint::setx(int x) {
    px=x;
}

void Gpoint::sety(int y) {
    py =y;
}

void Gpoint::move(int dx, int dy) {
    px += dx;
    py += dy;
}

void Gpoint::move(Gpoint& dpoint) {
    move(dpoint.x(),dpoint.y());
}

Gpoint Gpoint::traslate(int dx, int dy) {
    Gpoint newPoint = *this;
    newPoint.move(dx, dy);
    return newPoint;
}

Gpoint Gpoint::traslate(Gpoint& point) {
    Gpoint newPoint = *this;
    newPoint.move(point);
    return newPoint;
}


bool Gpoint::operator==(Gpoint& point) {
    return (point.x()== px && point.y() == py );
}

Gpoint Gpoint::operator-() {
    Gpoint point;
    point.px = -px;
    point.py = -py;
    return point;
}

Gpoint Gpoint::operator-(Gpoint& point) {
    Gpoint npoint;
    npoint.px = px - point.px;
    npoint.py = py - point.py;
    return npoint;
}

Gpoint Gpoint::operator+(Gpoint& point) {
    Gpoint npoint;
    npoint.px = px + point.px;
    npoint.py = py + point.py;
    return npoint;
}

Gpoint& Gpoint::operator+=(Gpoint& point) {
    px += point.px;
    py += point.py;
    return *this;
}

Gpoint& Gpoint::operator-=(Gpoint& point) {
    px -= point.px;
    py -= point.py;
    return *this;
}