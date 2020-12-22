#ifndef MENU_H
#define MENU_H
#include "frame.h"
#include "../basics/list.h"
#include "../basics/font.h"
#include "../basics/gbitmap.h"

enum eTypeMenuItem {
    tmSeparator, tmItem
};


typedef void (eventOnClickItem) (Sframe* sender);

class SmenuItem : public Grect {
public:
    SmenuItem(Grect rect, string &caption, int id);
    SmenuItem(string caption, int id);
    string strCaption;
    eTypeMenuItem typeMenuItem;
    GgraphicsBitmap *bitmap;
    int mid;
    eventOnClickItem *onClickItem;
    ~SmenuItem();
};

class Smenu : public Sframe {
public:
    Smenu(Grect rect, Sframe *aparent);
    void addItem(string str,int subid, eTypeMenuItem typeMenuItem);
    void addItem(SmenuItem *menuItem);
    void repaint();
    void onMouseMove(Gpoint &point);
    void onClickDown(Gpoint &point);
    int width;
    int height;
    Gcolor bkColor;
    ~Smenu();
private:
    int mid;
    Clist *subItems;
    Gfont *font;
    int itemindex;
    void getSize();
    Sframe *parent;
};

#endif

