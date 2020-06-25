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

void onScrollByGlistView(Gcontrol *control, int x, int y);

GlistView::GlistView() {
    setClassName("GlistView");
}

GlistView::GlistView(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("GlistView");
    color = 0xFFFFFFFF;
    selected = 0xa8bed5;
    focus = false;
    itemHeight = 20;
    charWidth = 10;
    maxWidth = 0;
    OnChangeItem = 0;
}

void GlistView::repaint() {
    int curPosDraw = itemIndex - scrollBarVertical->position;
    int curPosDrawBef = itemindexBefore - scrollBarVertical->position;
    if (itemindexBefore != itemIndex && (curPosDrawBef < maxItemsPaint && curPosDrawBef >= 0))
        paintData(itemindexBefore, 0, itemindexBefore - scrollBarVertical->position, color);
    if (curPosDraw < maxItemsPaint && curPosDraw >= 0) {
        if (focus) paintData(itemIndex, 0, itemIndex - scrollBarVertical->position, selected);
        else paintData(itemIndex, 0, itemIndex - scrollBarVertical->position, 0xd4d4d4);
    }
    paintVertical = false;
    paintLeft = false;
    if (refreshLine == false) {
        graphics->repaint(scrollBarVertical->graphics);
        graphics->repaint(scrollBarHorizontal->graphics);
    }
}

void GlistView::clickPressDown(int x, int y) {
    itemindexBefore = itemIndex;
    int dataTop = y - graphics->drawRect.top() - itemHeight;
    int itemSelected = dataTop / itemHeight;
    if (itemSelected >= maxItemsPaint || itemSelected >= items.count()) {
        itemIndex = -1;
        if (edit->focus) {
            changeCaption();
            return;
        }
    } else itemIndex = scrollBarVertical->position + itemSelected;
    if (itemindexBefore == itemIndex && edit->focus) {
        edit->clickPressDown(x);
        return;
    }
    if (edit->focus) {
        changeCaption();
        return;
    }
    refreshLine = true;
    refresh();
    refreshLine = false;
}

void GlistView::clickPressUp(int x, int y) {
    if (edit->focus) edit->clickPressUp(x);
}

void GlistView::mouseMove(int x, int y) {
    if (edit->focus) edit->mouseMove(x);
}

void GlistView::keyPress(int &key) {
    if (edit->focus) {
        edit->keyPress(key);
        if (key != 0x0A) {
            key = 0x0;
            return;
        }
        changeCaption();
        key = 0x0;
        return;
    }
    itemindexBefore = itemIndex;
    if (key == 0x28) { //Flecha abajo
        if (itemIndex < items.count() - 1) {
            itemIndex++;
            refreshLine = true;
            needScrollDown();
        }else return;
    } else if (key == 0x26) { //Flecha arriba
        if (itemIndex) {
            itemIndex--;
            refreshLine = true;
            needScrollUp();
        }else return;
    } else if (key == 0x27) {//Flecha derecha
        if (scrollBarHorizontal->position < scrollBarHorizontal->max) {
            scrollBarHorizontal->position++;
            scrollBarHorizontal->repaint();
            scrollBy(0, 1);
        }
        return;
    } else if (key == 0x25) {//Flecha Izquierda
        if (scrollBarHorizontal->position > 0) {
            scrollBarHorizontal->position--;
            scrollBarHorizontal->repaint();
            scrollBy(0, -1);
        }
        return;
    } else if (key == 0x71 && itemIndex != -1) {
        int indexSelected = itemIndex - scrollBarVertical->position + 1;
        edit->focus = true;
        edit->resize(columns[0]->drawWidth,edit->height);      
        edit->graphics->drawRect.setx(graphics->drawRect.x() + graphics->getdx() + edit->left);
        edit->graphics->drawRect.sety(graphics->drawRect.y() + graphics->getdy() + indexSelected * itemHeight);
        edit->text = items[itemIndex]->caption;
        edit->setCursorPosition(edit->text.Lenght());
        edit->refresh();
        return;
    } else return;
    refresh();
    refreshLine = false;
}

void GlistView::addColumn(Gcolumn* column) {
    GcolumnButton *newButton = new GcolumnButton(0, 0, column->width, itemHeight);
    newButton->initialized();
    newButton->parent = this;
    newButton->caption = column->caption;
    newButton->repaint();
    newButton->graphics->setx(maxWidth);
    column->component = (void*) newButton;
    insertControl(newButton);
    columns.add(column);
    maxWidth += column->width;
}

void GlistView::refresh() {
    repaint();
    if (refreshLine == false) kpaintArea(*this);
}

void GlistView::load() {
    graphics->backGroundColor(color);
    itemIndex = -1;
    itemindexBefore = -1;
    scrollBarVertical->max = items.count() - maxItemsPaint;
    scrollBarVertical->position = 0;
    scrollBarVertical->repaint();
    GcolumnButton *columnButton;
    maxWidth =0;
    for (int i = 0; i < columns.count(); i++) {
        columnButton = (GcolumnButton*) columns[i]->component;
        columnButton->graphics->setx(maxWidth);
        maxWidth += columnButton->width;
    }
    paintDataCaption();
    paintAllData(0, 0, 0, color);
    scrollBarHorizontal->max = (maxWidth / charWidth) - ((width - 24) / charWidth);
    scrollBarHorizontal->position =0;
    scrollBarHorizontal->repaint();
    graphics->repaint(scrollBarVertical->graphics);
    graphics->repaint(scrollBarHorizontal->graphics);
}

void GlistView::focused() {
    itemindexBefore = itemIndex;
    refresh();
}

void GlistView::scrollBy(int x, int y) {
    if (x < 0 && scrollBarVertical->position >= 0 && scrollBarVertical->max) {
        paintScrollUp();
        itemindexBefore = scrollBarVertical->position;
    } else if (x > 0 && (scrollBarVertical->position + maxItemsPaint) <= items.count()) {
        paintScrollDown();
        itemindexBefore = scrollBarVertical->position + (maxItemsPaint - 1);
    } else if (y < 0 && scrollBarHorizontal->position >= 0) {
        paintScrollRight();
        paintScrollColumn();
        paintDataCaption();
        paintVertical = true;
        itemindexBefore = itemIndex;
    } else if (y > 0 && scrollBarHorizontal->position <= scrollBarHorizontal->max && scrollBarHorizontal->max > 1) {
        paintScrollLeft();
        paintLeft = true;
        paintScrollColumn();
        paintDataCaption();
        paintVertical = true;
        itemindexBefore = itemIndex;
    } else return;
    refresh();
}

void GlistView::initialize() {
    graphics = new Gframe(left, top, width, height); //Lienzo de Dibujo del Componente
    graphics->paintBorder(1, 0x00939393);
    graphics->backGroundColor(color);
    graphics->reducedZone(0, 0);
    data = new Gfont(2, 0, width - 22, itemHeight); //Texto para mostrar cada linea del ListBox
    dataVertical = new Gfont(0, 0, charWidth, height - 22 - itemHeight);
    scrollBarVertical = new GscrollBar(width - 22, 0, 20, height - 24);
    scrollBarVertical->kind = sbVertical;
    scrollBarVertical->border = bsNone;
    scrollBarVertical->onScrollBy = (onScrollEvent*) onScrollByGlistView;
    scrollBarVertical->initialized();
    scrollBarVertical->parent = this;
    scrollBarHorizontal = new GscrollBar(0, height - 24, width - 22, 22);
    scrollBarHorizontal->kind = sbHorizontal;
    scrollBarHorizontal->border = bsNone;
    scrollBarHorizontal->onScrollBy = (onScrollEvent*) onScrollByGlistView;
    scrollBarHorizontal->initialized();
    scrollBarHorizontal->parent = this;
    maxItemsPaint = (graphics->trueRect.height() - 24 - data->height()) / itemHeight;
    edit = new GlineEdit(0, 0, width - 25, 20);
    edit->initialize();
    insertControl(scrollBarVertical);
    insertControl(scrollBarHorizontal);
}

GlistView::~GlistView() {
}

void GlistView::paintDataCaption() {
    int index = 0, i;
    GcolumnButton *button;
    for (i = 0; i < columns.count(); i++) {
        button = (GcolumnButton*) columns[i]->component;
        if (index < width) {
            bool cutColumn = false;
            if (button->graphics->left() < 0) cutColumn = true;
            graphics->repaintComponent(button->graphics);
            index += button->graphics->drawRect.width();
            columns[i]->drawWidth = button->graphics->drawRect.width();
            if (cutColumn) columns[i]->cutWidth = button->width - button->graphics->drawRect.width();
            else columns[i]->cutWidth = 0;
        } else break;
    }
}

void GlistView::paintData(int y, int x, int dy, Gcolor color) {
    int index = 0, i, indexColumn;
    int indexChar;
    string *textItem;
    int maxSizeText, minIndex = -1, minIndexChar;
    data->restoreZone();
    data->backGroundColor(color);
    for (i = 0; i < columns.count(); i++) {
        indexChar = columns[i]->cutWidth / charWidth;
        maxSizeText = (columns[i]->drawWidth / charWidth);
        if (minIndex < 0) {
            if (maxSizeText > 0) {
                minIndex = i;
                minIndexChar = indexChar;
            }
        }
        if (i == 0) textItem =  getItem(y,-1);
        else textItem = getItem(y, i-1);
        if (textItem)
            if (indexChar < textItem->Lenght())
        data->printLine(index, textItem->c_str() + indexChar, maxSizeText, 0x0, color);
        index += columns[i]->drawWidth;
        indexColumn = i;
        if (index > (width - 24)) break;
    }
    if (paintVertical) {
        if (paintLeft)
            paintDataVertical(scrollBarVertical->position, indexColumn, (maxSizeText - 3), width - 40, this->color);
        else
            paintDataVertical(scrollBarVertical->position, minIndex, minIndexChar, 0, this->color);
    }
    data->sety((dy + 1) * itemHeight);
    data->setx(0);
    graphics->repaint(data);
    if (refreshLine) {
        data->drawRect.sety(graphics->drawRect.y() + graphics->getdy() + data->y());
        data->drawRect.setx(graphics->drawRect.x() + graphics->getdx() + data->x());
        kpaintArea(data->drawRect, data->getGraphicsBitmap());
    }
}

void GlistView::paintDataVertical(int y, int dy, int x, int dx, Gcolor color) {
    dataVertical->restoreZone();
    dataVertical->backGroundColor(color);
    int itemsPaint = items.count() - scrollBarVertical->position;
    if (itemsPaint > maxItemsPaint) itemsPaint = maxItemsPaint;
    if (x < 0) return;
    string *textItem;
    for (int i = 0; i < itemsPaint; i++) {
        textItem = getItem(y+i, dy-1);
        if (textItem)
             if (x < textItem->Lenght())
        dataVertical->putc(0, i*itemHeight, 0x0, color,textItem->c_str()[x]);
    }
    dataVertical->setx(dx);
    dataVertical->sety(itemHeight);
    graphics->repaint(dataVertical);
}

void GlistView::paintAllData(int y, int x, int dy, Gcolor color) {
    int itemsPaint = items.count() - scrollBarVertical->position;
    if (itemsPaint > maxItemsPaint) itemsPaint = maxItemsPaint;
    for (int i = 0; i < itemsPaint; i++) {
        Gcolor valColor = color;
        if (itemIndex == scrollBarVertical->position + i) valColor = 0xFF00FF08;
        paintData(i + y, x, dy + i, valColor);
    }
}

void GlistView::needScrollDown() {
    if ((itemIndex - scrollBarVertical->position) >= maxItemsPaint) {
        scrollBarVertical->position += 1;
        paintScrollDown();
        refreshLine = false;
        scrollBarVertical->repaint();
        graphics->repaint(scrollBarVertical->graphics);
    }
}

void GlistView::needScrollUp() {
    if (itemIndex - scrollBarVertical->position < 0) {
        scrollBarVertical->position -= 1;
        paintScrollUp();
        refreshLine = false;
        scrollBarVertical->repaint();
        graphics->repaint(scrollBarVertical->graphics);
    }
}

void GlistView::paintScrollDown() {
    graphics->upLines(graphics->getdy() + itemHeight, itemHeight, maxItemsPaint * itemHeight + graphics->getdy());
}

void GlistView::paintScrollUp() {
    graphics->downLines(graphics->getdy() + itemHeight, itemHeight, (maxItemsPaint + 1) * itemHeight + graphics->getdy());
}

void onScrollByGlistView(Gcontrol *control, int x, int y) {
    ((GlistView*) control->parent)->scrollBy(x, y);
}

void GlistView::paintScrollLeft() {
    graphics->leftLines(graphics->getdx(), charWidth, width - 24);
}

void GlistView::paintScrollRight() {
    graphics->rightLines(graphics->getdx() + charWidth, charWidth, width - 24);
}

void GlistView::paintScrollColumn() {
    GcolumnButton *comp;
    for (int i = 0; i < columns.count(); i++) {
        comp = (GcolumnButton*) columns[i]->component;
        if (paintLeft) comp->graphics->move(-charWidth, 0);
        else comp->graphics->move(charWidth, 0);
    }
}

void GlistView::changeCaption() {
    bool accept = true;
    if (OnChangeItem) {
        OnChangeItem(this, items[itemindexBefore], edit->text, accept);
    }
    if (accept)
        items[itemindexBefore]->caption = edit->text;
    edit->focus = false;
    refreshLine = true;
    refresh();
    refreshLine = false;
}

string* GlistView::getItem(int index, int subItemIndex) {
    if (items.count() > index) {
        if (subItemIndex == -1) return &items[index]->caption;
        if (items[index]->subItem.count() > subItemIndex) {
            return items[index]->subItem[subItemIndex];
        }
        return 0;
    }
    return 0;
}
