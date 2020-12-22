//---------------------------------------------------------------------------
#ifndef grectH
#define grectH
#include "gpoint.h"
#include "gsize.h"

class GcanvasPainter;
class Grect : public Gpoint, public Gsize {
public:
    Grect();
    Grect(int x, int y, int width, int height);
    Grect(Gpoint &point, Gsize &size);
    Grect(Grect &rect);
    bool constains(int x, int y);
    bool constains(Gpoint &point);
    bool constains(Gpoint &point, Gsize &size);
    bool constains(Grect &rect);
    bool intersects(Grect &rect);
    void areaInstersection(Grect &xrect,Grect &yrect);
    int getRectsIntersects(Grect &rect, Grect *rects);
    Grect operator&(Grect &rect);
    Grect& operator &=(Grect &rect);
    Grect operator |(Grect &rect);
    Grect& operator |=(Grect &rect);
    bool operator ==(Grect &rect);
    void reduced(int rwidth, int reight);
    void reducedN(int rwidth, int reight);
    void reduced(Gpoint &point);
    void alignLeft(Grect &rect);
    void alignRight(Grect &rect);
    void alignTop(Grect &rect);
    void alignBottom(Grect &rect);
    void alignCenterHorizontal(Grect &rect);
    void alignCenterVertical(Grect &rect);
    void alignCenter(Grect rect);
    bool isEmpty();
    int left();
    int right();
    void setright(int right);
    int top();
    int bottom();
    void setbottom(int bottom);
    Gpoint center();
private:

};
//---------------------------------------------------------------------------
#endif
