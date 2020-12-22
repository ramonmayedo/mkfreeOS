#ifndef GCONTROLSCROLLED_H
#define GCONTROLSCROLLED_H

#include "GscrollBar.h"

class GcontrolScrolled : public Gcomponent {
public:
    GcontrolScrolled(int x, int y, int width, int height, Gcomponent *parent);
    int updateSizeContent(int width, int height);
    void changePositionComponents();
    void resize();
    GscrollBar *hScrollBar, *vScrollBar;
    Grect realDrawContent;
private:
    int sizeContentWidth, sizeContentHeight;
    bool visibleVerticalScrollBar, visibleHorizontalScrollBar;
   
};

#endif

