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

#include "graphicsbitmap.h"

GgraphicsBitmap::GgraphicsBitmap(int x, int y, int width, int height) : Grect(x, y, width, height) {
    glinearBuffer = new int[(width + 1)*(height + 1)];
    area.area = glinearBuffer;
    area.width = width;
    area.height = height;
    area.left = x;
    area.top = y;
}

void GgraphicsBitmap::paintPixel(int x, int y, Gcolor color) {
    glinearBuffer[y*width() + x] = color.valColor();
}

void GgraphicsBitmap::paintPixel(int x, int y, int color) {
    glinearBuffer[y*width() + x] = color;
}

void GgraphicsBitmap::paintPixel(int dx, int dy, Gcolor color, Grect& newZone) {
    int il = newZone.x() - x();
    int it = newZone.y() - y();
    glinearBuffer[(il + dx) + (dy+it)*width()] = color.valColor();    
}

int GgraphicsBitmap::getPixel(int dx, int dy, Grect& newZone) {
    int il = newZone.x() - x();
    int it = newZone.y() - y();
    return glinearBuffer[(il + dx) + (dy + it) * width()];
}

SvideoArea* GgraphicsBitmap::getArea() {
    return &area;
}

int* GgraphicsBitmap::getGraphicsBitmap() {
    return glinearBuffer;
}

void GgraphicsBitmap::setGraphicsBitmap(int *graphicsBitmap) {
    glinearBuffer = graphicsBitmap;
}

void GgraphicsBitmap::upLines(int index, int countLines, int top) {
    if (countLines + index >= height() || top >= height())
        return;
    else {
        int indexBuffer = index * width();
        int maxPixels = countLines * width();
        int sizeBuffer = top * width();
        for (int i = indexBuffer, j = indexBuffer + maxPixels; i < sizeBuffer; i++, j++) {
            glinearBuffer[i] = glinearBuffer[j];
        }
    }
}

void GgraphicsBitmap::downLines(int index, int countLines, int top) {
    if (countLines + index >= height())
        return;
    else {
        int indexBuffer = index * width();
        int maxPixels = countLines * width();
        int sizeBuffer = top * width();
        for (int i = sizeBuffer, j = sizeBuffer - maxPixels; i > indexBuffer; i--, j--) {
            glinearBuffer[i] = glinearBuffer[j];
        }
    }
}

void GgraphicsBitmap::leftLines(int index, int countLines, int top) {
    int movWidth = index - countLines + top - 1;
    for (int i = 0; i < height(); i++)
        for (int j = index; j < movWidth; j++)
            glinearBuffer[i * width() + j] = glinearBuffer[i * width() + j + countLines];
}

void GgraphicsBitmap::rightLines(int index, int countLines, int top) {
    int movWidth = index - countLines + top - 1;
    for (int i = 0; i < height(); i++)
        for (int j = movWidth; j > index; j--)
            glinearBuffer[i * width() + j] = glinearBuffer[i * width() + j - countLines];
}

GgraphicsBitmap::~GgraphicsBitmap() {
    delete(glinearBuffer);
}

