//---------------------------------------------------------------------------
#pragma hdrstop

#include "gpoint.h"
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

void Gpoint::setxy(int x, int y) {
    px = x;
    py = y;
}

void Gpoint::move(int dx, int dy) {
    px += dx;
    py += dy;
}

void Gpoint::move(Gpoint& dpoint) {
    move(dpoint.px, dpoint.py);
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
