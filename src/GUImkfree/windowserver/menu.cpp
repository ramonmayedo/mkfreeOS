#include "menu.h"
#include "../../library/includes/stdio.h"
#include "windowserver.h"

extern SwindowServer * windowServer;

SmenuItem::SmenuItem(Grect rect, string& caption, int id) : Grect(rect) {
    strCaption = caption;
    mid = id;
    onClickItem = 0;
    bitmap = 0;
}

SmenuItem::SmenuItem(string caption, int id) : Grect(0, 0, 0, 0) {
    strCaption = caption;
    mid = id;
    onClickItem = 0;
    bitmap = 0;
}

SmenuItem::~SmenuItem() {
    if (bitmap)
    delete(bitmap);
}

Smenu::Smenu(Grect rect, Sframe *aparent) : Sframe(rect) {
    parent = aparent;
    subItems = new Clist();
    font = new Gfont();
    height = rect.height();
    width = rect.width();
    frameType = ftMenu;
    itemindex = -1;
    bkColor = 0x00e3e3e3;
}

void Smenu::addItem(string str, int subid, eTypeMenuItem typeMenuItem) {
    SmenuItem *menuItem = new SmenuItem(str, subid);
    subItems->add(menuItem);
}

void Smenu::addItem(SmenuItem* menuItem) {
    subItems->add(menuItem);
}

void Smenu::repaint() {
    getSize();
    SmenuItem *menuItem;
    painter->cutRect = frameRect;
    painter->backgroundColor(bkColor);
    painter->borderColor(1, drawRect, 0x00a4a4a4);
    Grect draw(3, 4, width - 6, font->height + 6);
    for (int i = 0; i < subItems->count(); i++) {
        menuItem = (SmenuItem*) subItems->getItem(i);
        *(Grect*) menuItem = draw;
        painter->drawText(draw, 25, font, menuItem->strCaption.c_str(), 0x0, talignCenterTop);
        draw.move(0, font->height + 6);
        if (menuItem->bitmap) {
            menuItem->bitmap->setx(6);
            menuItem->bitmap->sety(i * (font->height + 6) + 7);
            painter->drawBitmapTransparent(menuItem->bitmap, *menuItem->bitmap, bkColor);
        }
    }
}

void Smenu::getSize() {
    SmenuItem *menuItem;
    int newWidth = 0, i = 0;
    for (i = 0; i < subItems->count(); i++) {
        menuItem = (SmenuItem*) subItems->getItem(i);
        newWidth = menuItem->strCaption.Lenght() * (font->width + 2) + 25;
        if (newWidth > width)
            width = newWidth + 25;
    }
    height = i * (font->height + 6) + 8;
    drawRect.setwidth(width);
    frameRect.setwidth(width);
    drawRect.setheight(height);
    frameRect.setheight(height);
    painter->canvas->resizeBitmap(width, height);
}

void Smenu::onMouseMove(Gpoint& point) {
    Gpoint position = point - drawRect;
    int item = position.y() / (font->height + 6);
    if (item >= subItems->count())
        item = -1;

    if (itemindex >= 0 && item != itemindex) {
        SmenuItem *itemSel = (SmenuItem*) subItems->getItem(itemindex);
        painter->drawRect(*itemSel, bkColor);
        painter->drawText(*itemSel, 25, font, ((SmenuItem*) subItems->getItem(itemindex))->strCaption.c_str(), 0x00, talignCenterTop);
        if (itemSel->bitmap)
            painter->drawBitmapTransparent(itemSel->bitmap, *itemSel->bitmap, bkColor);
        refreshRect(*itemSel);
    }

    if (item >= 0 && item != itemindex) {
        SmenuItem *itemSel = (SmenuItem*) subItems->getItem(item);
        painter->drawRectBorder(*itemSel, 0x00a8bed5, 0x00);
        painter->drawText(*itemSel, 25, font, ((SmenuItem*) subItems->getItem(item))->strCaption.c_str(), 0x00, talignCenterTop);
        if (itemSel->bitmap)
            painter->drawBitmapTransparent(itemSel->bitmap, *itemSel->bitmap, bkColor);
        refreshRect(*itemSel);
    }
    itemindex = item;
}

void Smenu::onClickDown(Gpoint& point) {
    if (itemindex >= 0) {
        SmenuItem *itemSel = (SmenuItem*) subItems->getItem(itemindex);
        windowServer->closeMenu();
        if (itemSel->onClickItem)
            itemSel->onClickItem(parent);
    }
}

Smenu::~Smenu() {
    SmenuItem *menuItem;
    for (int i = 0; i < subItems->count(); i++) {
        menuItem = (SmenuItem*) subItems->getItem(i);
        delete(menuItem);
    }
    subItems->clear();
    delete(subItems);
}

