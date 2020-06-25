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

#include "kgraphics.h"
#include "control.h"

void kpaintArea(Grect &drawRect, int *buffer) {
    SvideoArea area;
    area.area = buffer;
    area.left = drawRect.left();
    area.top = drawRect.top();
    area.width = drawRect.width();
    area.height = drawRect.height();
    paintArea(&area);
}

void kpaintArea(Gcontrol &control) {
    int *newBuffer;
    Grect rect = control.graphics->drawRect;
    int newWidth = control.width - rect.width();
    if (newWidth == 0 && rect.height() == control.height) {
        kpaintArea(rect, control.graphics->getGraphicsBitmap());
    } else {
        newBuffer = new int[(rect.width() + 1) * (rect.height()+1)];
        if (control.graphics->left() > 0 ) newWidth = 0;
        Grect newRect(control.graphics->left() + newWidth, control.graphics->top(), rect.width(), rect.height());
        for (int i = 0; i < rect.height(); i++)
            for (int j = 0; j < rect.width(); j++)
                newBuffer[i * rect.width() + j] = control.graphics->getPixel(j, i, newRect);
         kpaintArea(control.graphics->drawRect, newBuffer);
        delete(newBuffer);
    }
}