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

#include "control.h"
#include "../../library/includes/conio.h"
#include "kgraphics.h"

Gcontrol::Gcontrol() {
    initialize();
}

Gcontrol::Gcontrol(int x, int y, int cwidth, int cheight) {
    left = x;
    top = y;
    width = cwidth;
    height = cheight;
    initialize();
}

void Gcontrol::insertControl(Gcontrol* control) {
    int index = controls->indexOf(control);
    if (index != -1) return;
    controls->add(control);
}

void Gcontrol::removeControl(Gcontrol* control) {
    controls->remove(control);
}

int Gcontrol::controlCount() {
    return controls->count();
}

void Gcontrol::alignToPaint(Gcontrol* control, GAlign align) {
    switch (align) {
        case alLeft:
        {
            control->graphics->setx(0);
            return;
        }
        case alRight:
        {
            control->graphics->alignRight(graphics->trueRect);
            return;
        }
        case alTop:
        {
            control->graphics->sety(0);
            return;
        }
        case alBottom:
        {
            control->graphics->alignBottom(graphics->trueRect);
            return;
        }
        default: return;
    }
}

void Gcontrol::repaintAllControlChilds() {
    for (int i = 0; i < controlCount(); i++) {
        Gcontrol *childControl = (Gcontrol*) controls->getItem(i);
        if (childControl->align)
            alignToPaint(childControl, childControl->align);
        graphics->repaintComponent(childControl->graphics);
    }
}

void Gcontrol::repaintZone(Grect rect) {
    Gcontrol *child;
    Grect rectToPaint, rectChild;
    for (int k = 0; k < controlCount(); k++) {
        child = (Gcontrol*) controls->getItem(k);
        rectToPaint = child->graphics->drawRect & rect;
        rectChild = Grect(child->left, child->top, rectToPaint.width(), rectToPaint.height());
        int newLeft = child->graphics->drawRect.left() - rectToPaint.left();
        int newTop = child->graphics->drawRect.top() - rectToPaint.top();
        if (newLeft < 0) newLeft = -newLeft;
        if (newTop < 0) newTop = -newTop;
        rectChild.move(newLeft, newTop);
        for (int i = 0; i < rectToPaint.height(); i++)
            for (int j = 0; j < rectToPaint.width(); j++)
                graphics->paintPixel(j, i, child->graphics->getPixel(j, i, rectChild), rectToPaint); //
    }
    int *newBuffer = new int[(rect.width() + 1) * (rect.height() + 1)];
    for (int i = 0; i < rect.height(); i++)
        for (int j = 0; j < rect.width(); j++)
            newBuffer[i * rect.width() + j] = graphics->getPixel(j, i, rect);
    kpaintArea(rect, newBuffer);
    delete(newBuffer);
}

void Gcontrol::initialize() {
    align = alNone;
    graphics = 0;
    parent = 0;
    controls = new Clist();
    onClickDown = 0;
    onMouseMove = 0;
    onClickUp = 0;
    zindex = 1;
    onKeyPress = 0;
    border = bsNone;
    onFocused = 0;
    focus = false;
    onDoubleClick = 0;
}

Gcontrol* Gcontrol::getControl(int x, int y) {
    return getControl(this, x, y);
}

Gcontrol* Gcontrol::getControl(Gcontrol *control, int x, int y) {
    Gcontrol *selectedControl;
    for (int i = 0; i < control->controlCount(); i++) {
        selectedControl = (Gcontrol*) control->controls->getItem(i);
        if (selectedControl->graphics->drawRect.constains(x, y) && selectedControl->zindex) {
            if (selectedControl->controlCount()) selectedControl = getControl(selectedControl, x, y);
            return selectedControl;
        }
    }
    return control;
}

void* Gcontrol::reasignPositionsChilds() {
    getAllPosition(this, this->graphics->trueRect.left(), this->graphics->trueRect.top());
}

Gcontrol* Gcontrol::getAllPosition(Gcontrol* control, int dx, int dy) {
    Gcontrol *selectedControl;
    for (int i = 0; i < control->controlCount(); i++) {
        selectedControl = (Gcontrol*) control->controls->getItem(i);
        int fx = dx + selectedControl->graphics->left();
        int fy = dy + selectedControl->graphics->top();
        selectedControl->graphics->drawRect.setx(fx);
        selectedControl->graphics->drawRect.sety(fy);
        int dfx = fx + selectedControl->graphics->getdx();
        int dfy = fy + selectedControl->graphics->getdy();
        if (selectedControl->controlCount())
            getAllPosition(selectedControl, dfx, dfy);
    }
}



Gcontrol::~Gcontrol() {

}