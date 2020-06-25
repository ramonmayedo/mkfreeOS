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

#include "rect.h"
#include "templates.h"

Grect::Grect(): Gpoint(),Gsize() {
}

Grect::Grect(int x, int y, int width, int height) : Gpoint(x, y), Gsize(width, height) {

}

Grect::Grect(Gpoint &point, Gsize &size) : Gpoint(point.x(), point.y()), Gsize(size.width(), size.height()) {

}

Grect::Grect(Grect& rect) : Gpoint(rect.x(),rect.y()),Gsize(rect.width(),rect.height()) {

}

int Grect::left() {
    return x();
}

int Grect::top() {
    return y();
}

int Grect::right() {
    return (x()+width()-1);
}

void Grect::setright(int right) {
    setwidth(right - x() + 1);
}

int Grect::bottom() {
    return (y() + height() - 1);
}

void Grect::setbottom(int bottom) {
    setheight(bottom - y() + 1);
}

Gpoint Grect::center() {
    return Gpoint(x() + width() / 2, y() + height() / 2);
}

bool Grect::constains(int cx, int cy) {
    return (cx >= x() && cx <= right() && cy >= y() && cy <= bottom());
}

bool Grect::constains(Gpoint& point) {
    return (point.x() >= x() && point.x() <= right() && point.y() >= y() && point.y() <= bottom());
}

bool Grect::constains(Grect &rect) {
    return (left() <= rect.left() && right() >= rect.right() && top() <= rect.top() && bottom() >= rect.bottom());
}

bool Grect::intersects(Grect& rect) {
    return left() <= rect.right() && rect.left() <= right() && top() <= rect.bottom() && rect.top() <= bottom();
}

void Grect::areaInstersection(Grect& xrect, Grect& yrect) {
    int newLeft, newRight, newTop, newBottom;
    newLeft = max(xrect.left(), yrect.left());
    newRight = min(xrect.right(), yrect.right());
    newTop = max(xrect.top(), yrect.top());
    newBottom = min(xrect.bottom(), yrect.bottom());
    if (newLeft > newRight || newTop > newBottom) {
        setx(0);
        sety(0);
        setwidth(0);
        setheight(0);
        return;
    }
    setx(newLeft);
    sety(newTop);
    setwidth(newRight - newLeft + 1);
    setheight(newBottom - newTop + 1);
}

Grect Grect::operator&(Grect& rect) {
    Grect area;
    area.areaInstersection(*this, rect);
    return area;
}

Grect& Grect::operator&=(Grect& rect) {
    areaInstersection(*this, rect);
    return *this;
}

Grect Grect::operator|(Grect& rect) {
    if (isEmpty()) return rect;
    if (rect.isEmpty()) return *this;
    Grect newRect;
    int minLeft = min(left(), rect.left());
    int maxRight = max(right(), rect.right());
    int minTop = min(top(), rect.top());
    int maxBottom = max(bottom(), rect.bottom());
    newRect.setx(minLeft);
    newRect.sety(minTop);
    newRect.setright(maxRight);
    newRect.setbottom(maxBottom);
    return newRect;
}

Grect& Grect::operator|=(Grect& rect) {
    if (isEmpty()) return rect;
    if (rect.isEmpty()) return *this;
    int minLeft = (left() < rect.left() ? left() : rect.left());
    int maxRight = (right() > rect.right() ? right() : rect.right());
    int minTop = (top() < rect.top() ? top() : rect.top());
    int maxBottom = (bottom() > rect.bottom() ? bottom() : rect.bottom());
    setx(minLeft);
    sety(minTop);
    setright(maxRight);
    setbottom(maxBottom);
    return *this;
}

bool Grect::operator==(Grect& rect) {
    return (rect.width() == width() && rect.height() == height());
}

void Grect::reduced(int rwidth, int rheight) {
    setx(x() + rwidth / 2);
    setwidth(width() - rwidth);
    sety(y() + rheight / 2);
    setheight(height() - rheight);
}

void Grect::reduced(Gpoint& point) {
    setx(x() + point.x() / 2);
    setwidth(width() - point.x());
    sety(y() + point.y() / 2);
    setheight(height() - point.y());
}

void Grect::alignTop(Grect& rect) {
    sety(rect.y());
}

void Grect::alignBottom(Grect& rect) {
    sety(rect.height() - height());   
}

void Grect::alignLeft(Grect& rect) {
    setx(rect.x());
}

void Grect::alignRight(Grect& rect) {
    setx(rect.width() - width());
}

void Grect::alignCenterHorizontal(Grect& rect) {
     setx(rect.center().x()-width()/2);
}

void Grect::alignCenterVertical(Grect& rect) {
    sety(rect.center().y()-height()/2);
}

void Grect::alignCenter(Grect& rect) {
    alignCenterHorizontal(rect);
    alignCenterVertical(rect);
}

bool Grect::isEmpty() {
    return (width() == 0 && height() == 0);
}
