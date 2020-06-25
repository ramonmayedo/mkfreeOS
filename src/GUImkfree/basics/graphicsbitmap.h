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

#ifndef GRAPHICSBITMAP_H
#define GRAPHICSBITMAP_H
#include "rect.h"
#include "color.h"
#include "../../library/includes/conio.h"

class GgraphicsBitmap : public Grect {
public:
    GgraphicsBitmap(int x, int y, int width, int height);
    void paintPixel(int x, int y, Gcolor color);
    void paintPixel(int x, int y, int color);
    void paintPixel(int x, int y, Gcolor color, Grect& newZone);
    int getPixel(int x, int y, Grect& newZone);
    void downLines(int index, int countLines, int top);
    void upLines(int index, int countLines, int top);
    void leftLines(int index, int countLines, int top);
    void rightLines(int index, int countLines, int top);
    SvideoArea *getArea();
    int *getGraphicsBitmap();
    void setGraphicsBitmap(int *graphicsBitmap);
    ~GgraphicsBitmap();
private:
    int *glinearBuffer;
    SvideoArea area;
};

#endif

