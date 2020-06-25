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

#ifndef COLUMNS_H
#define COLUMNS_H
#include "cstring.h"
#include "slines.h"
#include "font.h"
#include "list.h"

class GlistItem {
public:
    GlistItem();
    ~GlistItem();
    string caption;
    int group;
    int focus;
    Gslines subItem;
};

class GlistItems {
public:
    GlistItems();
    ~GlistItems();
    void add(GlistItem *listItem);
    GlistItem* get(int index);
    GlistItem* operator[](int index);
    int count();
    void clear();
private:
    Clist *listItems;
};

class Gcolumn {
public:
    Gcolumn();
    Gcolumn(int cwidth);
    ~Gcolumn();
    string caption;
    int width;
    int index;
    int drawWidth;
    int cutWidth;
    Gslines items;
    bool selected;
    void *component;
private:
};

class Gcolumns {
public:
    Gcolumns();
    void add(Gcolumn *newColumn);
    Gcolumn *operator[](int index);
    Gcolumn *get(int index);
    int count();
    ~Gcolumns();
private:
    Clist *columns;
};

class GstatusPanel {
public:
    GstatusPanel();
    GstatusPanel(int width);
    int style;
    string text;
    int width;
};

class Gpanels {
public:
    Gpanels();
    void add(GstatusPanel *newPanel);
    GstatusPanel *operator[](int index);
    GstatusPanel *get(int index);
    int count();
    ~Gpanels();
private:
    Clist *panels;
};


#endif

