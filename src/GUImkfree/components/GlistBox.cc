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

#include "GlistBox.h"

void onScrollBy(Gcontrol *control, int x,int y);

GlistBox::GlistBox() {
    setClassName("GlistBox");
}

GlistBox::GlistBox(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("GlistBox");   //Valores por defecto
    color = 0xd4d4d4;
    selected = 0xFF00FF08;
    itemHeight = 20;
    itemIndex = -1;
}

void GlistBox::repaint() {
    if (itemindexBefore != itemIndex && (itemindexBefore - scrollBar->position < maxItemsPaint))
        paintData(itemindexBefore, 0, itemindexBefore - scrollBar->position, color);
    if (itemIndex - scrollBar->position < maxItemsPaint) {
        if (focus) paintData(itemIndex, 0, itemIndex - scrollBar->position, selected);
        else paintData(itemIndex, 0, itemIndex - scrollBar->position, 0xd4d4d4);
    }
    graphics->repaint(scrollBar->graphics);
}

void GlistBox::refresh() {
    repaint();
    kpaintArea(*this);
}

void GlistBox::scrollBy(int x, int y) {
    if (x < 0 && scrollBar->position >= 0 && scrollBar->max) {
        paintScrollUp();
        itemindexBefore = scrollBar->position;
    } else if (x > 0 && (scrollBar->position + maxItemsPaint) <= strings.count()) {
        paintScrollDown();
        itemindexBefore = scrollBar->position + (maxItemsPaint - 1);
    } else return;
    refresh();
}

void GlistBox::selectItem(int x, int y) {
    itemindexBefore = itemIndex;
    int dataTop = y - graphics->drawRect.top();
    int itemSelected = dataTop / itemHeight;
    if (itemSelected >= maxItemsPaint) itemIndex = -1;
    else itemIndex = scrollBar->position + itemSelected;
    refresh();
}

void GlistBox::focused() {
    itemindexBefore = itemIndex;
    refresh();
}

void GlistBox::keyPress(int &key) {
    itemindexBefore = itemIndex;
    if (key == 0x28) {    //Flecha abajo
        if (itemIndex < strings.count() - 1) {
            itemIndex++;
            needScrollDown();
        }
    } else if (key == 0x26) { //Flecha arriba
        if (itemIndex) {
            itemIndex--;
            needScrollUp();
        }
    } else return;
    refresh();
}

GlistBox::~GlistBox() {
    delete(graphics);
    delete(data);
    delete(scrollBar);
}

void onScrollBy(Gcontrol *control, int x, int y) {    //Conecta al scrollBar con el componente externo
    ((GlistBox*) control->parent)->scrollBy(x, y);
}

void GlistBox::initialized() {  
    graphics = new Gframe(left, top, width, height);  //Lienzo de Dibujo del Componente
    graphics->paintBorder(1, 0x0);
    graphics->backGroundColor(color);
    data = new Gfont(0, 0, width - 20, itemHeight);
    maxItemsPaint = graphics->trueRect.height() / itemHeight;    
    scrollBar = new GscrollBar(width - 22, 0, 20, height-2);//ScrollBar
    scrollBar->border = bsNone;
    scrollBar->onScrollBy = (onScrollEvent*) onScrollBy;
    scrollBar->kind = sbVertical;
    scrollBar->initialized();
    scrollBar->parent = this;    
    insertControl(scrollBar);  //Se insertan como componentes hijos
    repaintAllControlChilds(); //Actualizo la posicion de Todos los componentes dentro del lienzo
}

void GlistBox::paintScrollDown() {
    graphics->upLines(graphics->getdy(), itemHeight, (maxItemsPaint - 1) * itemHeight + graphics->getdy());
}

void GlistBox::paintScrollUp() {
    graphics->downLines(graphics->getdy() + itemHeight, itemHeight, (maxItemsPaint) * itemHeight + graphics->getdy());
}

void GlistBox::paintData(int y, int x, int dy, Gcolor color) {
    data->restoreZone();
    data->backGroundColor(color);
    data->print(strings[y]->c_str(), 0x0, color);
    data->sety(dy * itemHeight);
    graphics->repaint(data);
}

void GlistBox::paintAllItmes(int y, int x) {
    int itemsPaint = strings.count() - scrollBar->position;
    if (itemsPaint > maxItemsPaint) itemsPaint = maxItemsPaint;
    for (int i = 0; i < itemsPaint; i++) {
        Gcolor valColor = color;
        if (itemIndex == scrollBar->position + i) valColor = 0xFF00FF08;
        paintData(i, 0, i, valColor);
    }
}

void GlistBox::needScrollDown() {
    if ((itemIndex - scrollBar->position) >= maxItemsPaint) {
        scrollBar->position += 1;
        paintScrollDown();
        scrollBar->refresh();
    }
}

void GlistBox::needScrollUp() {
    if (itemIndex - scrollBar->position < 0) {
        scrollBar->position -= 1;
        paintScrollUp();
        scrollBar->refresh();
    }
}

void GlistBox::add(string str) {
    string *nuevo = new string(str);
    strings.add(nuevo);
    if (strings.count() > maxItemsPaint) {
        scrollBar->max++;
    }
}

void GlistBox::load() {
    paintAllItmes(0,0);
    scrollBar->repaint();
    graphics->repaint(scrollBar->graphics);
}