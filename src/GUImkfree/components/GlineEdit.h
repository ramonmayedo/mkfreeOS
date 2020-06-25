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

#ifndef GLINEEDIT_H
#define GLINEEDIT_H

#include "../basics/control.h"
#include "../basics/cstring.h"
#include "../basics/font.h"
#include "../basics/kgraphics.h"

class GlineEdit : public Gcontrol{
public:
    GlineEdit();
    GlineEdit(int x, int y, int width, int height);
    void setCursorPosition(int pos);
    void clickPressDown(int x);
    void clickPressUp(int x);
    void mouseMove(int x);
    void keyPress(int &key);
    void resize(int rwidth, int rheight);
    void focused();
    void initialize();
    void repaint();
    void refresh();
    bool deleteSelection();
    ~GlineEdit();
    Gcolor color;
    string text;
    int startSelection, sizeSelection;
private:
    int pcursor,position;
    Gfont *data;
    int charWidth, maxCharLine;
    Grect drawCursor;
    bool selectActive;
protected:
    void paintData(int x, Gcolor color);
    int isValidChar(int acar);
};

#endif

