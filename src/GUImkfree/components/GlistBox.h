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

#ifndef GLISTBOX_H
#define GLISTBOX_H
#include "../basics/control.h"
#include "../basics/slines.h"
#include "../basics/font.h"
#include "GscrollBar.h"

class GlistBox : public Gcontrol {
public:
    GlistBox();
    void repaint();
    void initialized();
    void refresh();
    GlistBox(int x, int y, int width, int height);
    void add(string str);
    void load();
    void scrollBy(int x,int y);  
    void selectItem(int x, int y);  //Funciones Externas
    void keyPress(int &key);
    void focused();
    ~GlistBox();
    Gslines strings;
    GscrollBar *scrollBar;
    int itemHeight;
    int itemIndex;
    Gcolor color,selected;
private:
    int maxItemsPaint;
    int itemindexBefore;
    Gfont *data;
protected:
    void paintAllItmes(int y, int x);
    void paintData(int y,int x, int dy, Gcolor color);
    void refreshScrollBox();
    void needScrollDown();
    void needScrollUp();
    void paintScrollDown();
    void paintScrollUp();
};

#endif

