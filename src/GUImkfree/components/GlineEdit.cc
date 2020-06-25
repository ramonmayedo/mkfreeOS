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

#include "GlineEdit.h"

GlineEdit::GlineEdit() {
    setClassName("GlineEdit");
}

GlineEdit::GlineEdit(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("GlineEdit");
    color = 0xFFFFFFFF;
    charWidth = 10;
    pcursor = 0;
    position =0;
    focus = false;
    selectActive = false;
    sizeSelection = 0;
    startSelection = -1;
}

void GlineEdit::repaint() {
    paintData(position, color);
    if (focus) {
        drawCursor.setx((pcursor - position) * charWidth);
        graphics->paint(drawCursor, 0x0);
    }
}

void GlineEdit::initialize() {
    graphics = new Gframe(left, top, width, height); //Lienzo de Dibujo del Componente    
    graphics->paintBorder(1, 0x0);
    if (border == bsSingle)
        graphics->reducedZone(2, 2); 
    graphics->backGroundColor(color);
    data = new Gfont(0, 0,graphics->trueRect.width(), graphics->trueRect.height()); //Texto para mostrar cada linea del ListBox
    maxCharLine = data->width() / charWidth;
    drawCursor.setheight(data->height()); //Cursor
    drawCursor.setwidth(2);
}

void GlineEdit::refresh() {
    repaint();
    kpaintArea(*this);
}

void GlineEdit::clickPressDown(int x) {
    int dataLeft = x - graphics->drawRect.left();
    dataLeft /= charWidth;
    dataLeft += position;
    if (dataLeft > text.Lenght()) {
        dataLeft = text.Lenght();
    }
    pcursor = dataLeft;
    refresh();
    
    if (selectActive) {
        selectActive = false;
        startSelection = -1;
        sizeSelection = 0;
    }else
        selectActive = true;
}

void GlineEdit::clickPressUp(int x) {
    selectActive = false;
}

void GlineEdit::mouseMove(int x) {
    if (selectActive) {
        int dataLeft = x - graphics->drawRect.left();
        dataLeft /= charWidth;
        dataLeft += position;
        if (dataLeft > text.Lenght()) dataLeft = text.Lenght();
        if (pcursor < dataLeft) {
            sizeSelection = dataLeft - pcursor;
            startSelection = pcursor;
        } else if (pcursor > dataLeft) {
            sizeSelection = pcursor - dataLeft;
            startSelection = dataLeft;
        }
        refresh();
    }
}

void GlineEdit::resize(int rwidth, int rheight) {
    if (rwidth != width || rheight != height) {
        height = rheight;
        width = rwidth;
        delete(graphics);
        delete(data);
        initialize();
    }
}

void GlineEdit::keyPress(int &key) {
    if (selectActive) return;
    if (key == 0x27) {
        if (pcursor < text.Lenght()) pcursor++;
        else return;
        if (pcursor - position > maxCharLine) position++;
    } else if (key == 0x25) {
        if (pcursor > 0) pcursor--;
        else return;
        if (pcursor - position < 0) position--;
    } else if (key == 0x08) { //Retroceso del carro
        if (!deleteSelection()) {
            if (pcursor > 0) {
                pcursor--;
                text.deleteChar(pcursor);
                if (pcursor - position < 0) position--;
            }else return;
        }
    } else if (key == 0x2E) {
        if (!deleteSelection()) {
            if (pcursor < text.Lenght()) text.deleteChar(pcursor);
            else return;
        }
    } else if (isValidChar(key)) {
        deleteSelection();
        text.insertChar(pcursor, key);
        pcursor += 1; //muevo el cursor
        if (pcursor - position > maxCharLine) position++;
    } else return;
    refresh();
}

void GlineEdit::focused() {
    if (sizeSelection) {
        selectActive = false;
        sizeSelection =0;
        startSelection=-1;
        repaint();
    } else graphics->paint(drawCursor, 0x00FFFFFFFF);
    
    kpaintArea(*this);
}

void GlineEdit::paintData(int x, Gcolor color) {
    data->restoreZone();
    data->backGroundColor(color);
    Gcolor backGroundColor;
    for (int i = 0; i < maxCharLine; i++) {
        int curCharPos = x + i;
        char charW = text.c_str()[curCharPos];
        if (charW == 0x0) break;
        if (curCharPos >= startSelection && curCharPos < startSelection + sizeSelection && selectActive)
            backGroundColor = 0x00FF00FF00;
        else
            backGroundColor = color;
        data->putc(i*charWidth, 0, 0x0, backGroundColor, charW);
    }
    graphics->repaint(data);
}

GlineEdit::~GlineEdit() {
}

int GlineEdit::isValidChar(int acar) {
    if ((acar >= 'a' && acar <= 'z') || (acar >= 'A' && acar <= 'Z') || (acar >= '0' && acar <= '9')) return 1;
    char format[18] = {' ','/','.','*'};
    for (int j = 0; j < 4; j++)
        if (acar == format[j]) return 1;
    return 0;
}

bool GlineEdit::deleteSelection() {
    if (sizeSelection) {
        text.deleteStrChar(startSelection, sizeSelection);
        if (startSelection != pcursor) pcursor = startSelection;
        selectActive = false;
        sizeSelection = 0;
        startSelection = -1;
        return true;
    }
    return false;
}

void GlineEdit::setCursorPosition(int pos) {
    pcursor = pos;
    if (pcursor > text.Lenght()) pcursor = text.Lenght();
    if (pcursor > maxCharLine) position =  pcursor - maxCharLine;
}
