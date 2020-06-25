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

#ifndef GLISTVIEW_H
#define GLISTVIEW_H
#include "../basics/control.h"
#include "../basics/font.h"
#include "../basics/columns.h"
#include "GscrollBar.h"
#include "GcolumnButton.h"
#include "GlineEdit.h"
typedef void (eventOnChangeItem) (Gcontrol* sender, GlistItem *item, string &newCaption,bool &accept);

class GlistView  : public Gcontrol{
public:
    GlistView();
    GlistView(int x, int y, int width, int height);
    void clickPressDown(int x, int y);
    void clickPressUp(int x,int y);
    void mouseMove(int x,int y);
    void keyPress(int &key); 
    void scrollBy(int x, int y);
    void focused();
    void repaint();
    void refresh();
    void load();
    void addColumn(Gcolumn *column);
    void initialize();
    eventOnChangeItem *OnChangeItem;
    ~GlistView();
    int itemIndex;
    Gcolumns columns;
    GlistItems items;
private:
    int itemindexBefore;
    GscrollBar *scrollBarVertical,*scrollBarHorizontal;
    Gcolor color, selected;
    Gfont *data, *dataVertical;
    GlineEdit *edit;
    int itemHeight;
    int charWidth;
    int maxItemsPaint;
    int maxWidth;
    bool paintVertical,paintLeft,refreshLine;
protected:
    void paintDataCaption();
    void paintData(int y, int x, int dy, Gcolor color);
    void paintDataVertical(int y, int dy, int x, int dx, Gcolor color);
    void paintAllData(int y, int x, int dy, Gcolor color);
    void needScrollDown();
    void needScrollUp();
    void paintScrollDown();
    void paintScrollUp();
    void paintScrollLeft();
    void paintScrollRight();
    void paintScrollColumn();
    void changeCaption();
    string* getItem(int index, int subItemIndex);
};

#endif

