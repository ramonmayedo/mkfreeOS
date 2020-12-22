#ifndef GCOMPONENT_H
#define GCOMPONENT_H
#include "GgraphicComponent.h"

class Gcomponent : public GgraphicComponent {
public:
    Gcomponent(int x, int y, int width, int height, GgraphicComponent *parent);
    void setLeft(int x);
    ~Gcomponent();
private:

};

#endif

