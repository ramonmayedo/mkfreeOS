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

#ifndef GTEXTEDIT_H
#define GTEXTEDIT_H
#include "../basics/control.h"
#include "../basics/font.h"
#include "GscrollBar.h"

#include "../basics/slines.h"


class GtextEdit : public Gcontrol{
public:
    GtextEdit();
    GtextEdit(int x, int y, int width, int height);
    void clickPressDown(int x, int y);
    void clickPressUp(int x, int y);
    void mouseMove();
    void keyPress(int &key);
    void focused();
    void initialized();
    void scrollBy(int x,int y);
    void repaint();
    void refresh();
    void addLine(char *str);
    bool loadFromFile(string fileName);
    Gcolor color;
    Gcolor lineSelected;
    string text;
    Gpoint pcursor;
    ~GtextEdit();
private:
    int lineHeight;
    int charWidth;
    int maxCharLines,maxItemsPaint;
    Gpoint selectionStart, selectionEnd;
    int selectionLengt;
    Gfont *data,*dataVertical;
    Gslines lines;
    Grect drawCursor;
    GscrollBar *scrollBarVertical,*scrollBarHorizontal;
    int  indexCursorBefore;
    bool refreshLine;
protected:
    int isValidChar(int acar);
    void needScrollDown();
    void needScrollUp();
    void needSrollLeft(); 
    void needScrollRight();
    void insertChar(int newChar);
    void backSpaceChar();
    void enterChar();
    void deleteChar();
    void paintData(int y,int x, int dy, Gcolor color);
    void paintDataVertical(int y,int x,int dx, Gcolor color);
    void paintBlankData(int y);
    void paintAllLine(int y, int x);
    void paintEnter(int y, int max);
    void paintDeleteLine(int y);
    void paintScrollDown();
    void paintScrollUp();
    void paintScrollLeft();
    void paintScrollRight();
};

#endif

