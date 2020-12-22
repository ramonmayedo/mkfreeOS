#ifndef COMPOSER_H
#define COMPOSER_H
#include "../basics/grect.h"
#include "cursor.h"
#include "../../library/includes/conio.h"
#include "window.h"
#include "../basics/list.h"

class Gcomposer {
public:
    Gcomposer(Clist *awindows);
    void changePositionCursor(int x, int y);
    void changeResolution(int width, int height);
    void drawWindow(Sframe *frame);
    void drawWindowsTop(Grect &rect);
    void drawRectIntersect(Grect &rect);
    void drawRectDirty(Sframe *frame, Grect rect);
    void changeCursor(eCursorType acursorType);
    Gpoint cursorPosition();
    void flushCursor();
    Gcursor *cursor;
private:
    Sframe *activeFrame;
    Clist *frames;
    SvideoArea areaScreen;
    int activeWindows;
    int dx, dy;
    int posY;
    int *buffer;
    Grect screen;
    void flushVideo(Grect *dirtyRect);
    void copyBuffers(int *src, int *dest, int height, int width, int srcWidth);
    Gcursor *cursors[9];
    eCursorType cursorType;
    Gpoint curPoint;
};

#endif

