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

#ifndef RECT_H
#define RECT_H
#include "point.h"
#include "size.h"

class Grect : public Gpoint, public Gsize {
public:
    Grect();
    Grect(int x, int y, int width, int height);
    Grect(Gpoint &point, Gsize &size);
    Grect(Grect &rect);
    bool constains(int x, int y);
    bool constains(Gpoint &point);
    bool constains(Gpoint &point, Gsize &size);
    bool constains(Grect &rect);
    bool intersects(Grect &rect);
    void areaInstersection(Grect &xrect,Grect &yrect);
    Grect operator&(Grect &rect);
    Grect& operator &=(Grect &rect);
    Grect operator |(Grect &rect);
    Grect& operator |=(Grect &rect);
    bool operator ==(Grect &rect);
    void reduced(int rwidth, int reight);
    void reduced(Gpoint &point);
    void alignLeft(Grect &rect);
    void alignRight(Grect &rect);
    void alignTop(Grect &rect);
    void alignBottom(Grect &rect);
    void alignCenterHorizontal(Grect &rect);
    void alignCenterVertical(Grect &rect);
    void alignCenter(Grect &rect);
    bool isEmpty();
    int left();
    int right();
    void setright(int right);
    int top();
    int bottom();
    void setbottom(int bottom);
    Gpoint center();
private:

};

#endif

