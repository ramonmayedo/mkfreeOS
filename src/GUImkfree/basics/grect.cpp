#include "grect.h"
#include "templates.h"

Grect::Grect() : Gpoint(), Gsize() {
}

Grect::Grect(int x, int y, int width, int height) : Gpoint(x, y), Gsize(width, height) {

}

Grect::Grect(Gpoint &point, Gsize &size) : Gpoint(point.px, point.py), Gsize(size.swidth, size.sheight) {
}

Grect::Grect(Grect& rect) : Gpoint(rect.px, rect.py), Gsize(rect.swidth, rect.sheight) {

}

int Grect::left() {
    return px;
}

int Grect::top() {
    return py;
}

int Grect::right() {
    return (px + swidth - 1);
}

void Grect::setright(int right) {
    swidth = (right - px + 1);
}

int Grect::bottom() {
    return (py + sheight - 1);
}

void Grect::setbottom(int bottom) {
    sheight = (bottom - py + 1);
}

Gpoint Grect::center() {
    return Gpoint(px + swidth / 2, py + sheight / 2);
}

bool Grect::constains(int cx, int cy) {
    return (cx >= px && cx <= right() && cy >= py && cy <= bottom());
}

bool Grect::constains(Gpoint& point) {
    return (point.px >= px && point.px <= right() && point.py >= py && point.py <= bottom());
}

bool Grect::constains(Grect &rect) {
    return (px <= rect.px && right() >= rect.right() && py <= rect.py && bottom() >= rect.bottom());
}

bool Grect::intersects(Grect& rect) {
    return px <= rect.right() && rect.px <= right() && py <= rect.bottom() && rect.py <= bottom();
}

void Grect::areaInstersection(Grect& xrect, Grect& yrect) {
    int newLeft, newRight, newTop, newBottom;
    newLeft = max(xrect.px, yrect.px);
    newRight = min(xrect.right(), yrect.right());
    newTop = max(xrect.py, yrect.py);
    newBottom = min(xrect.bottom(), yrect.bottom());
    if (newLeft > newRight || newTop > newBottom) {
        px = 0;
        py = 0;
        swidth = 0;
        sheight = 0;
        return;
    }
    px = newLeft;
    py = newTop;
    swidth = newRight - newLeft + 1;
    sheight = newBottom - newTop + 1;
}

int Grect::getRectsIntersects(Grect &rect, Grect *rects) {
    if (!(rect.intersects(*this))) {
        return 0;
    }
    int countPieces = 0;

    if (px > rect.px) {
        rects[countPieces] = Grect(rect.px, rect.py, px - rect.px, rect.sheight);
        countPieces++;
    }

    if (py > rect.py) {
        rects[countPieces] = Grect(px, rect.py, swidth, py - rect.py);
        countPieces++;
    }

    if (rect.right() > right()) {
        rects[countPieces] = Grect(right() + 1, rect.py, rect.right() - right(), rect.sheight);
        countPieces++;
    }

    if (rect.bottom() > bottom()) { //bottom
        rects[countPieces] = Grect(px, bottom() + 1, swidth, rect.bottom() - bottom());
        countPieces++;
    }
    return countPieces;
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
    int minLeft = min(px, rect.px);
    int maxRight = max(right(), rect.right());
    int minTop = min(py, rect.py);
    int maxBottom = max(bottom(), rect.bottom());
    newRect.px = minLeft;
    newRect.py = minTop;
    newRect.setright(maxRight);
    newRect.setbottom(maxBottom);
    return newRect;
}

bool Grect::operator==(Grect& rect) {
    return (rect.swidth == swidth && rect.sheight == sheight);
}

void Grect::reduced(int rwidth, int rheight) {
    px = (px + rwidth / 2);
    swidth = (swidth - rwidth);
    py = (py + rheight / 2);
    sheight = (sheight - rheight);
}

void Grect::reducedN(int rwidth, int reight) {
    swidth += rwidth;
    sheight += reight;
}

void Grect::reduced(Gpoint& point) {
    px = (px + point.px / 2);
    swidth = (swidth - point.px);
    py = (py + point.py / 2);
    sheight = (sheight - point.py);
}

void Grect::alignTop(Grect& rect) {
    py = rect.py;
}

void Grect::alignBottom(Grect& rect) {
    py = (rect.sheight - sheight);
}

void Grect::alignLeft(Grect& rect) {
    px = rect.px;
}

void Grect::alignRight(Grect& rect) {
    px = rect.swidth - swidth;
}

void Grect::alignCenterHorizontal(Grect& rect) {
    px = (rect.center().px - swidth / 2);
}

void Grect::alignCenterVertical(Grect& rect) {
    py = (rect.center().py - sheight / 2);
}

void Grect::alignCenter(Grect rect) {
    alignCenterHorizontal(rect);
    alignCenterVertical(rect);
}

bool Grect::isEmpty() {
    return (swidth == 0 || sheight == 0);
}