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

#include "GtoolBar.h"

GtoolButton::GtoolButton() {
    setClassName("GtoolButton");
}

GtoolButton::GtoolButton(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("GtoolButton");
    color = 0xc6c6c6;
    state=0;
}

void GtoolButton::initialized() {
    graphics = new Gframe(left, top, width, height); //Lienzo de Dibujo del Componente
    graphics->backGroundColor(color);
}

void GtoolButton::refresh() {
    repaint();
    kpaintArea(*this);
}

void GtoolButton::mouseMove(Gcontrol* sender, int x, int y) {
    state = 1;
    refresh();
    state =0;
}

void GtoolButton::repaint() {
    graphics->restoreZone();
    if (state == 0) {
        graphics->backGroundColor(color);
    } else {
        graphics->paintBorder(1,0x0);
        graphics->backGroundColor(0xd1d1d1);
    }
}

GtoolButton::~GtoolButton() {

}

GtoolButtons::GtoolButtons() {
    toolButtons = new Clist();
}

void GtoolButtons::addButton(GtoolButton* newToolButton) {
    toolButtons->add(newToolButton);
}

GtoolButton* GtoolButtons::getButton(int index) {
    return (GtoolButton*) toolButtons->getItem(index);
}

void GtoolButtons::Delete(GtoolButton* button) {
    toolButtons->remove(button);
    button->~GtoolButton();
}

GtoolButton* GtoolButtons::operator[](int index) {
    return (GtoolButton*) toolButtons->getItem(index);
}

int GtoolButtons::count() {
    return toolButtons->count();
}

GtoolButtons::~GtoolButtons() {

}

GtoolBar::GtoolBar() {
    setClassName("GtoolBar");
}

GtoolBar::GtoolBar(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("GtoolBar");
    color = 0xe1e1e1;
}

void GtoolBar::initialized() {
    graphics = new Gframe(left, top, width, height); //Lienzo de Dibujo del Componente
    graphics->backGroundColor(color);
    graphics->paintBorderBottom(1,true,0xc0c0c0);
}

void GtoolBar::refresh() {
    repaint();
    kpaintArea(*this);
}

void GtoolBar::repaint() {
    graphics->restoreZone();
    graphics->backGroundColor(color);
    graphics->paintBorderBottom(1, true, 0xc0c0c0);
    int index = 5;
    for (int i = 0; i < toolButtons.count(); i++) {
        if (index < width) {
            toolButtons[i]->graphics->setx(index);
            graphics->repaintComponent(toolButtons[i]->graphics);
            index += toolButtons[i]->width + 4;
        } else break;
    }
}

void GtoolBar::addToolButton(GtoolButton* newToolButton) {
    toolButtons.addButton(newToolButton);
    insertControl(newToolButton);
}

GtoolBar::~GtoolBar() {
}

