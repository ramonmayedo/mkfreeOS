#ifndef FRAME_H
#define FRAME_H
#include "../basics/grect.h"
#include "../basics/canvasPainter.h"

enum eFrameType {
    ftNone, ftWindow, ftMenu, ftToolBar,ftDesktop
};

class Sframe {
public:
    Grect frameRect, drawRect;
    eFrameType frameType;
    GcanvasPainter *painter;
    int keyShared;
    Sframe(Grect rect);
    Sframe(Grect rect, Gsize size);
    void refreshRect(Grect &rect);
    ~Sframe();
private:

};

#endif

