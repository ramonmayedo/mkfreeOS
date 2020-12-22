/*Copyright (C) 2019  Ram�n Mayedo Morales (ramonmayedo@gmail.com)

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

#ifndef GSCROLLBAR_H
#define GSCROLLBAR_H
#include "Gcomponent.h"

typedef void (onScrollEvent)(GgraphicComponent* sender,int pos);

enum scrollBarKind {
     kindVertical, kindHorizontal
};

class GscrollBar : public Gcomponent {
public:
    GscrollBar(int x, int y, int width, int heigh,scrollBarKind kind ,Gcomponent *parent);
    void clickPressUp(int x, int y);
    void clickPressDown(int x, int y) ;
    void mouseMove(int x, int y);
    void draw(Grect &rect);
    onScrollEvent *onScrollBy;
    ~GscrollBar();
    Gcolor color;
    int scrollPosition;
    int min;
    int inc;
    scrollBarKind kind;
    void setmax(int amax);
    void resize();
private:
     Grect bup1, bdown1;
     Grect cursor1,bscroll1;
     GgraphicsBitmap *bup, *bdown;
     float paso;
     bool clickedCursor;
     Gpoint posClick;
     int max;
protected:
    void scrollForce(Gpoint newPosition, Gpoint actualPosition);
    void scrolled(Gpoint newPosition, Gpoint actualPosition);

};

#endif

