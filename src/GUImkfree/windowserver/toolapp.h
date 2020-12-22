#ifndef TOOLAPP_H
#define TOOLAPP_H
#include "window.h"
#include "../basics/list.h"
#include "../basics/gbitmap.h"

class StoolButton;
typedef void (eventToolOnClick) (StoolButton *sender);

class StoolButton : public Grect {
public:
    StoolButton(Grect rect, GgraphicsBitmap *abitmap);
    ~StoolButton();
    int bid;
    GgraphicsBitmap *bitmap;
    eventToolOnClick *onPressDown;
private:
    bool selected;
   
};

class StoolApp: public Grect{
public:
    StoolApp(Sframe *frame, Grect rect);
    void addToolButton(StoolButton *button);
    void deleteButton(int bid);
    void repaint();
    void onClickDown(Gpoint point);
    void deselectButton();
    void selectButton(int bid);
    void paintButton(StoolButton *button, bool selected);
    StoolButton *bottonSelected;
    ~StoolApp();
private:
    Clist buttons;
    GcanvasPainter *canvas;
    Sframe *parent;
    Grect buttonLeft, buttonRight, toolButtonRect;
    void repaintButton(StoolButton *button);
    void repaintAll();
    int indexPaint, maxToolVisible;
};

#endif

