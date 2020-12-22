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

#include "GcontrolScrolled.h"
#include "../../library/includes/stdio.h"

void onScrolled(GgraphicComponent *sender, int inc) {
    ((GcontrolScrolled*) sender)->changePositionComponents();
}

void onResizeControlScrolled(GgraphicComponent *sender){
    ((GcontrolScrolled*) sender)->resize();
}

GcontrolScrolled::GcontrolScrolled(int x, int y, int width, int height, Gcomponent *parent) : Gcomponent(x, y, width, height, parent) {
    vScrollBar = new GscrollBar(0, 0, 24, height - 24, kindVertical, this);
    hScrollBar = new GscrollBar(0, 0, width - 24, 24, kindHorizontal, this);
    hScrollBar->onScrollBy = onScrolled;
    vScrollBar->onScrollBy = onScrolled;
    vScrollBar->componentAlign = caRight;
    vScrollBar->componentPosition = cpFixed;
    hScrollBar->componentAlign = caBottom;
    hScrollBar->componentPosition = cpFixed;
    onResize = onResizeControlScrolled;
    resize();
}

int GcontrolScrolled::updateSizeContent(int width, int height) {
    sizeContentHeight = height;
    sizeContentWidth = width;
}

void GcontrolScrolled::changePositionComponents() {
    offsetPosition = Gpoint(-hScrollBar->scrollPosition, -vScrollBar->scrollPosition);
    rebuildComponents();
    refresh();
}

void GcontrolScrolled::resize() {
    realDrawContent.setwidth(childRect.width() - 24);
    realDrawContent.setheight(childRect.height() - 24);
    int maxX = sizeContentHeight - realDrawContent.height();
    int maxY = sizeContentWidth - realDrawContent.width();

    if (maxX) {
        vScrollBar->setmax(maxX);
        vScrollBar->inc = 10;
    }
    if (maxY) {
        hScrollBar->setmax(maxY);
        hScrollBar->inc = 10;
    }
    offsetPosition = Gpoint(-hScrollBar->scrollPosition, -vScrollBar->scrollPosition);
}
