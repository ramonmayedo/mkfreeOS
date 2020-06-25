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

#include "GscrollBar.h"

void onClickDown_bup(Gcontrol *control);
void onClickDown_bdown(Gcontrol *control);

GscrollBar::GscrollBar() {
    setClassName("GscrollBar");
}

GscrollBar::GscrollBar(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("GscrollBar");
    //Propiedades inicializables por defecto
    color = 0x84351a;
    max = 0;
    inc = 0;
    min = 0;
    position = 0;
    onScrollBy = 0;
}

void GscrollBar::repaint() {
    if (max <= 0) max = 0;
    if (position < 0) position = 0;
    else if (position > max) position = max;
    graphics->repaint(bscroll->graphics);
    if (max > 0)cursorResize();
}

GscrollBar::~GscrollBar() {

}

void onClickDown_bdown(Gcontrol* control) {
    Gcontrol *parent = ((Gbutton*) control)->parent;
    int curMove = 1;
    ((GscrollBar*) parent)->position += 1;
    if (((GscrollBar*) parent)->position > ((GscrollBar*) parent)->max) curMove = 0;
    ((GscrollBar*) parent)->refresh();
    if (((GscrollBar*) parent)->onScrollBy) {
        if (((GscrollBar*) parent)->kind == sbVertical)
            ((GscrollBar*) parent)->onScrollBy(parent, curMove, 0);
        else
            ((GscrollBar*) parent)->onScrollBy(parent, 0, curMove);
    }
}

void onClickDown_bup(Gcontrol* control) {
    Gcontrol *parent = ((Gbutton*) control)->parent;
    int curMove = -1;
    ((GscrollBar*) parent)->position -= 1;
    if (((GscrollBar*) parent)->position < 0) curMove = 0;
    ((GscrollBar*) parent)->refresh();
    if (((GscrollBar*) parent)->onScrollBy) {
        if (((GscrollBar*) parent)->kind == sbVertical)
            ((GscrollBar*) parent)->onScrollBy(parent, curMove, 0);
        else
            ((GscrollBar*) parent)->onScrollBy(parent, 0, curMove);
    }
}

void GscrollBar::refresh() {
    repaint();
    kpaintArea(*this);
}

void GscrollBar::cursorResize() {
    int sizeCursor, factor, movCursor, oldTop;
    if (kind == sbVertical) {
        sizeCursor = (bscroll->height - max);
        factor = position;
        movCursor = cursor->top + factor;
        oldTop = cursor->top;
        cursor->graphics->sety(movCursor);
        cursor->graphics->setheight(sizeCursor);
        graphics->repaint(cursor->graphics);
        cursor->graphics->sety(oldTop);
    } else {
        sizeCursor = (bscroll->width - max);
        factor = position;
        movCursor = cursor->left + factor;
        oldTop = cursor->left;
        cursor->graphics->setx(movCursor);
        cursor->graphics->setwidth(sizeCursor);
        graphics->repaint(cursor->graphics);
        cursor->graphics->setx(oldTop);
    }
}

void GscrollBar::initialized() {
    //Lienzo Principal de Dibujo
    graphics = new Gframe(left, top, width, height);
    graphics->restoreZone();
    if (border) graphics->paintBorder(1, 0x0);
    graphics->backGroundColor(color);
    //Barra de scroll
    if (kind == sbVertical)
        bscroll = new Gpanel(0, 30, width, height - 60);
    else
        bscroll = new Gpanel(30, 0, width - 60, height);
    bscroll->repaint();
    bscroll->graphics->backGroundColor(0x00e3e3e3);
    bscroll->parent = this;
    //Boton de Bajar
    if (kind == sbVertical)
        bdown = new Gbutton(0, graphics->trueRect.height() - 30, width, 30);
    else
        bdown = new Gbutton(graphics->trueRect.width() - 30, 0, 30, height);
    bdown->caption = ">";
    bdown->border = bsNone;
    bdown->repaint();
    bdown->onClickDown = (eventOnClick*) onClickDown_bdown;
    bdown->parent = this;
    //Boton de Subir
    if (kind == sbVertical)
        bup = new Gbutton(0, 0, graphics->trueRect.width(), 30);
    else
        bup = new Gbutton(0, 0, 30, graphics->trueRect.height());

    bup->caption = "<";
    bup->border = bsNone;
    bup->repaint();
    bup->onClickDown = (eventOnClick*) onClickDown_bup;
    bup->parent = this;
    //Cursor
    if (kind == sbVertical)
        cursor = new Gpanel(0, 30, width, bscroll->height);
    else
        cursor = new Gpanel(30, 0, bscroll->width, height);
    cursor->repaint();
    cursor->graphics->backGroundColor(0x008d8d8d);
    cursor->parent = this;   
    insertControl(bup);//Se insertan los componentes
    insertControl(bdown);
    insertControl(bscroll);
    insertControl(cursor);   
    repaintAllControlChilds();  
    repaint();  //Se actualiza la posicion del scroll
}