#include "gsize.h"

Gsize::Gsize() {
    sheight = 0;
    swidth = 0;
}

Gsize::Gsize(int width, int height) {
    swidth = width;
    sheight = height;
}

int Gsize::height() {
    return sheight;
}

int Gsize::width() {
    return swidth;
}

void Gsize::setwidth(int width) {
    swidth = width;
}

void Gsize::setheight(int height) {
    sheight = height;
}

bool Gsize::operator==(Gsize& size) {
    return (sheight == size.sheight && swidth == size.swidth);
}

Gsize Gsize::operator-(Gsize& size) {
    Gsize nsize;
    nsize.swidth = swidth - size.swidth;
    nsize.sheight = sheight - size.sheight;
    return nsize;
}

Gsize& Gsize::operator-=(Gsize& size) {
    swidth -= size.swidth;
    sheight -= size.sheight;
    return *this;
}

Gsize Gsize::operator+(Gsize& size) {
    Gsize nsize;
    nsize.swidth = swidth + size.swidth;
    nsize.sheight = sheight + size.sheight;
    return nsize;
}

Gsize& Gsize::operator+=(Gsize& size) {
    swidth += size.swidth;
    sheight += size.sheight;
    return *this;
}