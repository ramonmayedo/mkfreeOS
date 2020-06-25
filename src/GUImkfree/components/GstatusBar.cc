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

#include "GstatusBar.h"

GstatusBar::GstatusBar() {
     setClassName("GstatusBar");
}

GstatusBar::GstatusBar(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("GstatusBar");
    color = 0x00e3e3e3;
    charWidth = 10;
}

void GstatusBar::addPanel(GstatusPanel* panel) {
    panels.add(panel);
}

void GstatusBar::initialized() {
    graphics = new Gframe(left, top, width, height); //Lienzo de Dibujo del Componente
    graphics->paintBorderTop(1,true, 0x008d8d8d);
    graphics->backGroundColor(color);
    data = new Gfont(0, 1, width, height-2); 
}

void GstatusBar::refresh() {
     repaint();
     kpaintArea(*this);
}

void GstatusBar::repaint() {
    int index = 5;
    for (int i = 0; i < panels.count(); i++) {
        if (index < width) {
            data->restoreZone();
            data->backGroundColor(color);
            data->setwidth(panels[i]->width);
            int maxChar = panels[i]->width / charWidth;
            data->printLine(0,panels[i]->text.c_str(),maxChar,0x0,color);
            data->paintBorderRight(1,true,0x00939393);
            data->setx(index);
            graphics->repaint(data);
            index += panels[i]->width + 2;
        } else break;
    }
}

GstatusBar::~GstatusBar() {
}

