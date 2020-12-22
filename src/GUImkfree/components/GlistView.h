#ifndef GLISTVIEW_H
#define GLISTVIEW_H
#include "GcontrolScrolled.h"
#include "../basics/columns.h"

class GlistView : GcontrolScrolled {
public:
    GlistView(int x, int y, int width, int height,  Gcomponent* parent);
    void addColumn(Gcolumn *column);
    void draw(Grect &rect);
    ~GlistView();
private:
    Gcolumns columns;
    int maxWidth;
};

#endif

