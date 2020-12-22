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
#include "GlistView.h"
#include "Gbutton.h"

void onPaintListView(GgraphicComponent *sender, Grect &rect) {
    ((GlistView*) sender)->draw(rect);
}

GlistView::GlistView(int x, int y, int width, int height, Gcomponent* parent) : GcontrolScrolled(x, y, width, height, parent) {
    onPaint = onPaintListView;
    maxWidth = 0;
}

void GlistView::draw(Grect &rect) {
    canvas->cutRect = rect;
    canvas->pointRect = position;
    canvas->drawRect(Grect(0, 0, realRect.width(), realRect.height()), 0x00FFFFFF);
    canvas->borderColor(1, realRect, 0x004f4f4f);
  /*  int left = 0;
    for (int i = 0; i < columns.count(); i++) {
        ((Gbutton*) columns[i]->component)->setLeft(left + );
        left += columns[i]->width;
    }*/
   // updateSizeContent(maxWidth, realRect.height());
}

void GlistView::addColumn(Gcolumn* column) {
    Gbutton *button = new Gbutton(0, 0, column->width, 20, this);
    button->caption = column->caption;
    column->component = (void*) button;
    columns.add(column);
    button->setLeft(maxWidth);
    //button->componentPosition = cpFixed;
    maxWidth += column->width;
    updateSizeContent(maxWidth, realRect.height());
}

GlistView::~GlistView() {
}

