#ifndef GBUTTON_H
#define GBUTTON_H
#include "Gcomponent.h"

class Gbutton :public Gcomponent {
public:
    Gbutton(int x, int y, int width, int height, Gcomponent *parent);
    void clickDown();
    void clickUp();
    void draw(Grect &eventDrawRect);
    ~Gbutton();
    string caption;
    int bstate;
private:
    int style;
};

#endif

