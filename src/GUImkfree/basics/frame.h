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

#ifndef FRAME_H
#define FRAME_H
#include "rect.h"
#include "graphicsbitmap.h"
#include "point.h"
#include "color.h"

enum GAlign {
    alNone, alTop, alBottom, alLeft, alRight, alClient, alCustom
};

/*typedef void (drawEventOnClick) (int x, int y);
typedef void (drawEventOnKeyPress) (int key);
typedef void (drawEventOnFocused) (void);
typedef void (drawEventOnMouseMove) (int x, int y);*/

class Gframe : public GgraphicsBitmap {
public:
    Grect trueRect;
    Grect drawRect;
    Gframe(int x, int y, int width, int height);
    void backGroundColor(Gcolor color);
    void paint(Grect& rect,Gcolor color);
    void paint(Gframe *frame);
    void repaint(Gframe *frame);
    void repaintComponent(Gframe *frame);
    void paintBorder(int bwidth,Gcolor color);
    void paintBorderLeft(int bwidth, bool reduced, Gcolor color);
    void paintBorderRight(int bwidth, bool reduced, Gcolor color);
    void paintBorderTop(int bwidth, bool reduced, Gcolor color);
    void paintBorderBottom(int bwidth, bool reduced, Gcolor color);
    void reducedZone(int width,int height);
    int getdx();
    int getdy();
    void restoreZone();
/*    drawEventOnClick *drawOnClickDown;
    drawEventOnClick *drawOnClickUp;
    drawEventOnKeyPress *drawOnKeyPress;
    drawEventOnFocused *drawOnFocused;
    drawEventOnMouseMove *drawOnMouseMove;*/
    ~Gframe();
private:  

};

#endif

