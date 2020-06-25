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

#include "GtextEdit.h"
#include "../../library/includes/stdio.h"

void onScrollByGtextEdit(Gcontrol *control, int x,int y);

GtextEdit::GtextEdit() {
    setClassName("GtextEdit");
}

GtextEdit::GtextEdit(int x, int y, int width, int height) : Gcontrol(x, y, width, height) {
    setClassName("GtextEdit");
    color = 0xFFFFFFFF;
    lineSelected = 0xFFe9eff8;
    lineHeight = 20;
    charWidth = 10;
    refreshLine = false;
}

void GtextEdit::repaint() {
    if (indexCursorBefore != pcursor.y() && (indexCursorBefore - scrollBarVertical->position < maxItemsPaint))
        paintData(indexCursorBefore, scrollBarHorizontal->position, indexCursorBefore - scrollBarVertical->position, color);
    if (pcursor.y() - scrollBarVertical->position < maxItemsPaint) {
        paintData(pcursor.y(), scrollBarHorizontal->position, pcursor.y() - scrollBarVertical->position, lineSelected);
    }
    if (refreshLine == false) {
        graphics->repaint(scrollBarVertical->graphics);
        graphics->repaint(scrollBarHorizontal->graphics);
    }
}

void GtextEdit::clickPressDown(int x, int y) {
    int itemsPaint = lines.count() - scrollBarVertical->position;
    if (itemsPaint > maxItemsPaint) itemsPaint = maxItemsPaint;
    indexCursorBefore = pcursor.y();
    int dataTop = y - graphics->drawRect.top();
    int dataLeft = x - graphics->drawRect.left();
    dataTop /= lineHeight;
    dataLeft /= charWidth;
    dataLeft+= scrollBarHorizontal->position;
    if (dataTop >= itemsPaint) dataTop = itemsPaint - 1;
    int dataScrollTop = dataTop + scrollBarVertical->position;
    if (dataLeft >= lines[dataScrollTop]->Lenght()) dataLeft = lines[dataScrollTop]->Lenght();
    pcursor.setx(dataLeft);
    pcursor.sety(dataScrollTop);
    refreshLine = true;
    refresh();
    refreshLine = false;
}

void GtextEdit::clickPressUp(int x, int y) {

}

void GtextEdit::mouseMove() {

}

void GtextEdit::keyPress(int &key) {
    indexCursorBefore = pcursor.y();
    if (isValidChar(key)) //Insertar caracter
        insertChar(key);
    else if (key == 0x08) //Retroceso del carro
        backSpaceChar();
    else if (key == 0x2E) //Delete
        deleteChar();
    else if (key == 0x0A) //Salto de Linea
        enterChar();
    else if (key == 0x28) { //flecha abajo
        if (pcursor.y() < lines.count() - 1) {
            pcursor.move(0, 1);
            refreshLine = true;
            if (lines[pcursor.y()]->Lenght() < pcursor.x())
                pcursor.setx(lines[pcursor.y()]->Lenght());
            needScrollDown();
        }else return;
    } else if (key == 0x26) { //Flecha arriba
        if (pcursor.y()) {
            pcursor.move(0, -1);
            refreshLine = true;
            if (lines[pcursor.y()]->Lenght() < pcursor.x())
                pcursor.setx(lines[pcursor.y()]->Lenght());
            needScrollUp();
        }else return;
    } else if (key == 0x25) { //flecha izquierad
        if (pcursor.x() > 0){
            refreshLine = true;
            pcursor.move(-1, 0);
            needScrollRight();
        }else if (pcursor.y() > 0) {
            refreshLine = true;
            pcursor.move(0, -1);
            pcursor.setx(lines[pcursor.y()]->Lenght());
            needScrollUp();
        }else return;
    } else if (key == 0x27) { //flecha derecha
        if (pcursor.x() < lines[pcursor.y()]->Lenght()) {
            refreshLine = true;
            pcursor.move(1, 0);
            needSrollLeft();
        }else if (pcursor.y() < lines.count() - 1) {
            refreshLine = true;
            pcursor.move(0, 1);
            pcursor.setx(0);
            needScrollDown();
        }else return;
    } else return;
    refresh();
    refreshLine = false;
}

void GtextEdit::focused() {
    indexCursorBefore = pcursor.y();
    repaint();
}

void GtextEdit::refresh() {
    repaint();
    if (refreshLine == false) kpaintArea(*this);
}

void GtextEdit::addLine(char* str) {
    string *nuevo = new string(str);
    lines.add(nuevo);
    if (lines.count() > maxItemsPaint) scrollBarVertical->max++;
    int newScroll = nuevo->Lenght() - maxCharLines;
    if (scrollBarHorizontal->max < newScroll) scrollBarHorizontal->max = newScroll;
}

bool GtextEdit::loadFromFile(string fileName) {
    FILE *fileToLoad = fopen(fileName.c_str(), "r+");
    if (fileToLoad) {
        lines.clear();
        char *buffer = new char[1025];
        buffer[1024] = 0x0;
        int index = 0;
        while (!feof(fileToLoad)) {
            fread(buffer, 1024, 1, fileToLoad);
            for (int i = 0; i < 1024; i++) {
                if (buffer[i] == 0xd) {
                    buffer[i] = 0x0;
                    addLine(&buffer[index]);
                    index = i+2;
                    i++;
                }
            }
        }
        paintAllLine(0, 0);
        scrollBarHorizontal->refresh();
        scrollBarVertical->refresh();
        graphics->repaint(scrollBarVertical->graphics);
        graphics->repaint(scrollBarHorizontal->graphics);
    }
    return false;
}

void GtextEdit::needScrollDown() {
    if ((pcursor.y() - scrollBarVertical->position) >= maxItemsPaint) {
        refreshLine = false;
        scrollBarVertical->position += 1;
        paintScrollDown();      
    }
    if (lines.count() >=  maxItemsPaint - 2) scrollBarVertical->repaint();
}

void GtextEdit::needScrollUp() {
    if (pcursor.y() - scrollBarVertical->position < 0) { //scrollBar->position   
        refreshLine = false;
        scrollBarVertical->position -= 1;
        paintScrollUp();
        scrollBarVertical->repaint();
    }
}

void GtextEdit::needSrollLeft() {
    if (pcursor.x() - scrollBarHorizontal->position > maxCharLines) {
        refreshLine = false;
        scrollBarHorizontal->position += 1;
        paintScrollLeft();
        paintDataVertical(scrollBarVertical->position, pcursor.x() - 1, maxCharLines - 1, color);
        scrollBarHorizontal->repaint();
    }
}

void GtextEdit::needScrollRight() {
    if (pcursor.x() - scrollBarHorizontal->position < 0) {
        refreshLine =false;
        scrollBarHorizontal->position -= 1;
        paintScrollRight();
        paintDataVertical(scrollBarVertical->position, pcursor.x(), 0, color);
        scrollBarHorizontal->repaint();
    }
}

GtextEdit::~GtextEdit() {
}

void GtextEdit::initialized() {
    graphics = new Gframe(left, top, width, height);  //Lienzo de Dibujo del Componente
    graphics->paintBorder(1, 0x0);
    graphics->backGroundColor(color);  
    data = new Gfont(0, 0, width - 24, lineHeight);   //Texto para mostrar cada linea del ListBox
    dataVertical = new Gfont(0, 0, charWidth, height - 24);  
    scrollBarVertical = new GscrollBar(width - 22, 0, 20, height-24);
    scrollBarVertical->kind = sbVertical;
    scrollBarVertical->border = bsNone;
    scrollBarVertical->onScrollBy = (onScrollEvent*) onScrollByGtextEdit;
    scrollBarVertical->initialized();
    scrollBarVertical->parent = this; 
    scrollBarHorizontal = new GscrollBar(0,height - 24 ,width-22 ,22);
    scrollBarHorizontal->kind = sbHorizontal;
    scrollBarHorizontal->border = bsNone;
    scrollBarHorizontal->onScrollBy = (onScrollEvent*) onScrollByGtextEdit;
    scrollBarHorizontal->initialized();
    scrollBarHorizontal->parent = this;
    maxCharLines = data->width() / charWidth;
    maxItemsPaint = (graphics->trueRect.height() - 24) / lineHeight;
    drawCursor.setheight(lineHeight);//Cursor
    drawCursor.setwidth(2);
    string *newLine = new string();
    lines.add(newLine);
    insertControl(scrollBarVertical);
    insertControl(scrollBarHorizontal);
    repaint();
}

int GtextEdit::isValidChar(int acar) {
    if ((acar >= 'a' && acar <= 'z') || (acar >= 'A' && acar <= 'Z') || (acar >= '0' && acar <= '9')) return 1;
    char format[18] = {' '};
    for (int j = 0; j < 18; j++)
        if (acar == format[j]) return 1;
    return 0;
}

void GtextEdit::scrollBy(int x, int y) {
    if (x < 0 && scrollBarVertical->position >= 0 && scrollBarVertical->max) {
        paintScrollUp();
        indexCursorBefore = scrollBarVertical->position;
    } else if (x > 0 && (scrollBarVertical->position + maxItemsPaint) <= lines.count()) {
        paintScrollDown();
        indexCursorBefore = scrollBarVertical->position + (maxItemsPaint - 1);
    } else if (y < 0 && scrollBarHorizontal->position >= 0) {
        paintScrollRight();
        paintDataVertical(scrollBarVertical->position, scrollBarHorizontal->position, 0, color);
        indexCursorBefore = pcursor.y();
    } else if (y > 0 && scrollBarHorizontal->position <= scrollBarHorizontal->max && scrollBarHorizontal->max > 1) {
        paintScrollLeft();
        paintDataVertical(scrollBarVertical->position, scrollBarHorizontal->position + maxCharLines - 1, maxCharLines - 1, color);
        indexCursorBefore = pcursor.y();
    } else return;
    refresh();
}

void onScrollByGtextEdit(Gcontrol *control, int x, int y) {
    ((GtextEdit*) control->parent)->scrollBy(x, y);
}

void GtextEdit::insertChar(int newChar) {
    refreshLine = true;
    lines[pcursor.y()]->insertChar(pcursor.x(), newChar);
    pcursor.move(1, 0); //muevo el cursor
    if (pcursor.x() - scrollBarHorizontal->position > maxCharLines) { //Si es el fin de la linea
        needSrollLeft();
        if (scrollBarHorizontal->max < (lines[pcursor.y()]->Lenght() - 1))
            scrollBarHorizontal->max++;
        scrollBarHorizontal->position += 1;
        scrollBarHorizontal->repaint();
    } else if (lines[pcursor.y()]->Lenght() > maxCharLines) {
        if (scrollBarHorizontal->max < (lines[pcursor.y()]->Lenght() - 1))
            scrollBarHorizontal->max++;
        scrollBarHorizontal->refresh();
    }
}


void GtextEdit::backSpaceChar() {
    if (pcursor.x() > 0) { //Si se mantiene el cursor en la misma linea
        refreshLine = true;
        pcursor.move(-1, 0); //Se mueve hacia la izquierda
        lines[pcursor.y()]->deleteChar(pcursor.x());
        needScrollRight();
    } else  if (pcursor.y() > 0) { //Ha que subir linea
        string *lineBefore = lines[pcursor.y() - 1];
        int lenghtBefore = lineBefore->Lenght();
        lineBefore->insertString(lines[pcursor.y()]);
        lines.Delete(pcursor.y()); //Se elimina la linea       
        pcursor.move(lenghtBefore, -1); //Se mueve a la linea anterior
        paintDeleteLine(pcursor.y());
    }
}

void GtextEdit::enterChar() {
    string *lineBefore = lines[pcursor.y()];
    int sizeStr = lineBefore->Lenght() - pcursor.x();
    string *newLine = new string(lineBefore->c_str() + pcursor.x(), sizeStr);
    lines.insert(pcursor.y() + 1, newLine);
    lines[pcursor.y()]->deleteStrChar(pcursor.x(), sizeStr);
    pcursor.move(0, 1);
    pcursor.setx(0);
    if (lines.count() > maxItemsPaint) scrollBarVertical->max += 1;
    paintEnter(pcursor.y(), maxItemsPaint);
    needScrollDown();   
}

void GtextEdit::deleteChar() {
    if (pcursor.x() < lines[pcursor.y()]->Lenght()){
        refreshLine = true;
        lines[pcursor.y()]->deleteChar(pcursor.x());
    } else if (pcursor.y() < lines.count() - 1) { //Ultimo caracter de la Linea       
        lines[pcursor.y()]->insertString(lines[pcursor.y() + 1]);
        lines.Delete(pcursor.y() + 1); //Se elimina la linea
        paintDeleteLine(pcursor.y());
    }
}

void GtextEdit::paintData(int y,int x, int dy, Gcolor color) {
    data->restoreZone();
    data->backGroundColor(color);
    data->printLine(0,lines[y]->c_str() + x, lines[y]->Lenght() - x, 0x0, color);
    if (focus == true && color == lineSelected) {
        drawCursor.setx((pcursor.x() - scrollBarHorizontal->position) * charWidth);
        data->paint(drawCursor, 0x0);
    }
    data->sety(dy * lineHeight);
    graphics->repaint(data);  
    if (refreshLine) {
        data->drawRect.sety(graphics->drawRect.y() + graphics->getdy() + data->y());
        data->drawRect.setx(graphics->drawRect.x() + graphics->getdx() + data->x());
        kpaintArea(data->drawRect, data->getGraphicsBitmap());
    }
     
}

void GtextEdit::paintDataVertical(int y, int x, int dx, Gcolor color) {
    dataVertical->restoreZone();
    dataVertical->backGroundColor(color);
    int itemsPaint = lines.count() - scrollBarVertical->position;
    if (itemsPaint > maxItemsPaint) itemsPaint = maxItemsPaint;
    bool emptyChar = true;
    for (int i = 0; i < itemsPaint; i++) {
        if (lines[y + i]->Lenght() > x) {
            char charPaint = lines[y + i]->c_str()[x];
            dataVertical->putc(0, i*lineHeight, 0x0, color, charPaint);
            emptyChar = false;
        }
    }
    if (emptyChar == true)  scrollBarHorizontal->max--;
    dataVertical->setx(dx * charWidth);
    graphics->repaint(dataVertical);
}

void GtextEdit::paintBlankData(int y) {
    Grect blankData(0, y * lineHeight, data->width(), data->height());
    graphics->paint(blankData, color);
}

void GtextEdit::paintAllLine(int y, int x) {
    int itemsPaint = lines.count() - scrollBarVertical->position;
    if (itemsPaint > maxItemsPaint) itemsPaint = maxItemsPaint;
    for (int i = 0; i < itemsPaint; i++) {
        paintData(i + y, x, i, color);
    }
}

void GtextEdit::paintEnter(int y, int max) {
    int itemsPaint = (y - scrollBarVertical->position);
    if (y != lines.count() - 1) //Si es la ultima linea no es necesario bajar
        graphics->downLines(graphics->getdy()+ (itemsPaint + 1) * lineHeight, lineHeight, (max) * lineHeight + graphics->getdy());
}

void GtextEdit::paintDeleteLine(int y) {
    int itemsPaint = (y - scrollBarVertical->position);
    int lineEnd = y + (maxItemsPaint - itemsPaint - 1);
    scrollBarVertical->max -= 1;
    if (itemsPaint < 0) {                  //Si es la linea 0 la que se borro
        indexCursorBefore = pcursor.y();   //No hay que actualizar el cursor
        scrollBarVertical->position -= 1;          //Se bajo una linea
        scrollBarVertical->repaint();              //Se actualiza
        return;
    }
    //Si no hay mas linea a subir, y hay lineas hacia arriba las bajo
    if (lineEnd >= lines.count() && scrollBarVertical->position) {
        paintEnter(scrollBarVertical->position, itemsPaint + 1);  //Bajo las lineas hasta el cursor
        paintData(scrollBarVertical->position - 1,scrollBarHorizontal->position, 0, color);     //Pinto la primera linea que fue la que se bajo
        scrollBarVertical->position -= 1;                         //Se bajo una linea
        scrollBarVertical->repaint(); //Se actualiza
        if (y == lines.count() - 1)          //si es ultima linea
            indexCursorBefore = pcursor.y(); //No es neceasario actualizar el cursor
        return;
    }        
    if (y == lines.count() - 1) {           //si es ultima linea se rellena en blanco
        paintBlankData(itemsPaint + 1);       
        indexCursorBefore = pcursor.y();    //No es neceasario actualizar el cursor
        return;
    }
    if (itemsPaint + 1 < maxItemsPaint)       //Si hay lineas para subir 
        graphics->upLines(graphics->getdy()+(itemsPaint + 1) * lineHeight, lineHeight, (maxItemsPaint - 1) * lineHeight + graphics->getdy());
  
    if (lineEnd == lines.count()) {         //Si es la ultima linea linea la que se subio
        paintBlankData(maxItemsPaint - 1);  //Se pinta en blanco
        indexCursorBefore = pcursor.y();    //No es neceasario actualizar el cursor
        return;
    }
    
    if (lineEnd < lines.count()) {                      //Si hay mas lineas hacia abajo
        paintData(lineEnd,scrollBarHorizontal->position, (maxItemsPaint - 1), color); //Subo proxima Linea
        scrollBarVertical->repaint();                           //Se actualiza
    } 
}

void GtextEdit::paintScrollDown() {
    graphics->upLines(graphics->getdy(), lineHeight, (maxItemsPaint - 1) * lineHeight + graphics->getdy());
}

void GtextEdit::paintScrollUp() {
    graphics->downLines(graphics->getdy() + lineHeight, lineHeight, (maxItemsPaint) * lineHeight + graphics->getdy());
}

void GtextEdit::paintScrollLeft() {
    graphics->leftLines(graphics->getdx(), charWidth, (maxCharLines) * charWidth + graphics->getdx());
}

void GtextEdit::paintScrollRight() {
    graphics->rightLines(graphics->getdx() + charWidth, charWidth, (maxCharLines) * charWidth + graphics->getdx());
}