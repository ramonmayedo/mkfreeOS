//---------------------------------------------------------------------------
#pragma hdrstop

#include "gcolor.h"
 Gcolor::Gcolor() {
    vcolor = 0;
}

Gcolor::Gcolor(int color) {
    vcolor = color;
}

Gcolor::Gcolor(unsigned char r, unsigned char g, unsigned char b) {
    vcolor = (0xff000000 | (r << 16) | (g << 8) | b);
}

Gcolor::Gcolor(unsigned char r, unsigned char g, unsigned char b, char a) {
    vcolor = ((a << 24) | (r << 16) | (g << 8) | b);
}

int Gcolor::valColor() {
    return vcolor;
}

unsigned char Gcolor::red() {
    return (vcolor >> 16)&0xFF;
}

unsigned char Gcolor::blue() {
    return vcolor&0xFF;
}

unsigned char Gcolor::green() {
    return (vcolor >> 8)&0xFF;
}

unsigned char Gcolor::alpha() {
    return (vcolor >> 24)&0xFF;
}

void Gcolor::setcolor(int color) {
    vcolor = color;
}

void Gcolor::setred(char r) {
    vcolor = vcolor&0xFF00FFFF | (r << 16);
}

void Gcolor::setblue(char b) {
    vcolor = vcolor&0xFFFFFF00 | b;
}

void Gcolor::setgreen(char g) {
    vcolor = vcolor&0xFFFF00FF | (g << 8);
}

void Gcolor::setalpha(char a) {
    vcolor = vcolor&0x00FFFFFF | (a << 24);
}

Gcolor Gcolor::grayScale() {
    int grayValue = (red()+green()+blue())/3;
    return Gcolor(grayValue,grayValue,grayValue);
}

Gcolor Gcolor::darkned() {
    return Gcolor(red()/2,green()/2,blue()/2);
}

Gcolor Gcolor::lightned() {

}

Gcolor Gcolor::inverted() {
    return Gcolor(~red(), ~green(), ~blue());
}

Gcolor Gcolor::widthAlpha(char a) {
    return Gcolor((0x00FFFFFF & vcolor) | a << 24);
}

bool Gcolor::operator==(Gcolor& color) {
    return (vcolor==color.vcolor);
}

bool Gcolor::operator!=(Gcolor& color) {
    return (vcolor!=color.vcolor);
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
