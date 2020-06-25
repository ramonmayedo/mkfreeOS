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

#include "GmainBar.h"

GmainBarButton::GmainBarButton() {
    setClassName("GmainBarButton");
}

GmainBarButton::GmainBarButton(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("GmainBarButton");
    color = 0xc6c6c6;
    state=0;
}

void GmainBarButton::initialized() {
    graphics = new Gframe(left, top, width, height); //Lienzo de Dibujo del Componente
    graphics->backGroundColor(color);
}

void GmainBarButton::refresh() {
    repaint();
    kpaintArea(*this);
}

void GmainBarButton::mouseMove(Gcontrol* sender, int x, int y) {
    state = 1;
    refresh();
    state =0;
}

void GmainBarButton::repaint() {
    graphics->restoreZone();
    if (state == 0) {
        graphics->backGroundColor(color);
    } else {
        graphics->paintBorder(1,0x0);
        graphics->backGroundColor(0xd1d1d1);
    }
}

GmainBarButton::~GmainBarButton() {

}

GmainBarButtons::GmainBarButtons() {
    toolButtons = new Clist();
}

void GmainBarButtons::addButton(GmainBarButton* newToolButton) {
    toolButtons->add(newToolButton);
}

GmainBarButton* GmainBarButtons::getButton(int index) {
    return (GmainBarButton*) toolButtons->getItem(index);
}

GmainBarButton* GmainBarButtons::operator[](int index) {
    return (GmainBarButton*) toolButtons->getItem(index);
}

void GmainBarButtons::Delete(GmainBarButton* button) {
    toolButtons->remove(button);
}

int GmainBarButtons::count() {
    return toolButtons->count();
}

GmainBarButtons::~GmainBarButtons() {

}

GmainBar::GmainBar() {
    setClassName("GmainBar");
}

GmainBar::GmainBar(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("GmainBar");
    color = 0xe1e1e1;
    activeButton = 0;
}

void GmainBar::initialized() {
    graphics = new Gframe(left, top, width, height); //Lienzo de Dibujo del Componente
    graphics->backGroundColor(color);
    graphics->paintBorderBottom(1,true,0xc0c0c0);
}

void GmainBar::refresh() {
    repaint();
    kpaintArea(*this);
}

void GmainBar::repaint() {
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

void GmainBar::addToolButton(GmainBarButton* newToolButton) {
    toolButtons.addButton(newToolButton);
    insertControl(newToolButton);
}

void GmainBar::deleteToolButton(GmainBarButton *toolButton){
    removeControl(toolButton);
    toolButtons.Delete(toolButton);
}

GmainBar::~GmainBar() {
}

