#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H
#include "window.h"
#include "toolapp.h"
#include "menu.h"

class SmainToolBar : public Sframe {
    
public:
    SmainToolBar(Grect rect);
    void addApp(int wid, GgraphicsBitmap *bitmap);
    void activeApp(int wid);
    void closeApp(int wid);
    void changeIcoApp(int wid, GgraphicsBitmap *bitmap);
    void deselctApp();
    void repaint();
    ~SmainToolBar();
    void onClickDown(Gpoint point);
private:
    Grect buttonStart;
    StoolApp *toolApp;
    //Smenu *menuApp;
};

#endif

