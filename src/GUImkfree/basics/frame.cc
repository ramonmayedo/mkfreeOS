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

#include "frame.h"
#include "control.h"

Gframe::Gframe(int x, int y, int width, int height) : GgraphicsBitmap(x, y, width, height) {
    trueRect = Grect(x, y, width, height);
    drawRect = trueRect;
/*    drawOnClickDown = 0;
    drawOnClickUp = 0;
    drawOnFocused = 0;
    drawOnKeyPress = 0;
    drawOnMouseMove = 0;*/
}

void Gframe::backGroundColor(Gcolor color) {
    for (int i = 0; i < trueRect.height(); i++)
        for (int j = 0; j < trueRect.width(); j++)
            paintPixel(j, i, color, trueRect);
}

void Gframe::paint(Grect& rect, Gcolor color) {
    Grect newRect = rect;
    newRect.move(trueRect);
    newRect &= trueRect;
    for (int i = 0; i < newRect.height(); i++)
        for (int j = 0; j < newRect.width(); j++)
            paintPixel(j + rect.left(), i + rect.top(), color, trueRect);
}

void Gframe::repaintComponent(Gframe* frame) {
    Grect newRect = *(Grect*) frame;
    newRect.move(trueRect);
    newRect &= trueRect;
    frame->drawRect.setwidth(newRect.width());
    frame->drawRect.setheight(newRect.height());
    newRect.setx(((Grect*) frame)->left());
    newRect.sety(((Grect*) frame)->top());
    int left = frame->left();
    if (left < 0) {
        left = 0;
        newRect.move(frame->width() - newRect.width(), 0);
        frame->drawRect.setx(drawRect.x() + getdx());
    } else {
        frame->drawRect.setx(drawRect.x() + getdx() + frame->left());
        frame->drawRect.sety(drawRect.y() + getdy() + frame->top());
    }
    for (int i = 0; i < newRect.height(); i++)
        for (int j = 0; j < newRect.width(); j++)
            paintPixel(j + left, i + frame->top(), frame->getPixel(j, i, newRect), trueRect);
}

void Gframe::repaint(Gframe *frame) {
    Grect newRect = *(Grect*) frame;
    newRect.move(trueRect);
    newRect &= trueRect;
        for (int i = 0; i < newRect.height(); i++)
        for (int j = 0; j < newRect.width(); j++)
            paintPixel(j + frame->left(), i + frame->top(), frame->getPixel(j, i, *(Grect*) frame), trueRect);
}

void Gframe::paint(Gframe* frame) {
    frame->drawRect.move(trueRect);
    frame->drawRect &= trueRect;
    for (int i = 0; i < frame->drawRect.height(); i++)
        for (int j = 0; j < frame->drawRect.width(); j++)
            paintPixel(j + frame->left(), i + frame->top(), frame->getPixel(j, i, *(Grect*) frame), trueRect);
}

void Gframe::paintBorder(int bwidth, Gcolor color) {
    trueRect.reduced(bwidth*2,bwidth*2);
    paintBorderLeft(bwidth,false,color);
    paintBorderRight(bwidth,false,color);
    paintBorderTop(bwidth,false,color);
    paintBorderBottom(bwidth,false,color);
}

void Gframe::paintBorderLeft(int bwidth,bool reduced, Gcolor color) {
    if (reduced)trueRect.reduced(bwidth*2,0);
     for (int i=0;i<height();i++)
        for (int j=0;j<bwidth;j++)
           paintPixel(j,i,color);
}

void Gframe::paintBorderRight(int bwidth,bool reduced, Gcolor color) {
    if (reduced)trueRect.reduced(bwidth*2,0);
    for (int i = 0; i < height(); i++)
        for (int j = bwidth; j > 0; j--)
            paintPixel(width() - j, i, color);
}

void Gframe::paintBorderTop(int bwidth,bool reduced, Gcolor color) {
    if (reduced)trueRect.reduced(0,bwidth*2);
    for (int i = 0; i < bwidth; i++)
        for (int j = 0; j < width(); j++)
            paintPixel(j, i, color);
}

void Gframe::paintBorderBottom(int bwidth,bool reduced, Gcolor color) {
    if (reduced)trueRect.reduced(0,bwidth*2);
    for (int i = bwidth; i > 0; i--)
        for (int j = 0; j < width() ; j++)
            paintPixel(j,height() -  i, color);
}

void Gframe::reducedZone(int rwidth, int rheight) {
    trueRect.reduced(rwidth,rheight);
}

void Gframe::restoreZone() {
    trueRect = *(Grect*)this;
}

int Gframe::getdx() {
    return (width() - trueRect.width()) / 2;
}

int Gframe::getdy() {
    return (height() - trueRect.height()) / 2;
}

Gframe::~Gframe() {
}