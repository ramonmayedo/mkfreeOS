#include "composer.h"
#include "../basics/templates.h"
#include "../../library/includes/stdio.h"

Gcomposer::Gcomposer(Clist *awindows) {
    frames = awindows;
    buffer = 0;
    cursors[ctArrow] = new Gcursor("a/kernel/cursors/arrow.png");
    cursors[ctResizeDiagonalRight] = new Gcursor("a/kernel/cursors/resizeDR.png", -2, -2);
    cursors[ctResizeVertical] = new Gcursor("a/kernel/cursors/resizeV.png", 0, -2);
    cursors[ctResizeHorizontal] = new Gcursor("a/kernel/cursors/resizeH.png", -2, 0);
    cursors[ctResizeDiagonalLeft] = new Gcursor("a/kernel/cursors/resizeDL.png", -2, -2);
    cursor = cursors[ctArrow];
}

void Gcomposer::changePositionCursor(int x, int y) {
    dx += x;
    dy += y;
    if (dx > areaScreen.trueWidth) dx = areaScreen.trueWidth;
    else if (dx < 0) dx = 0;
    if (dy > areaScreen.trueHeight) dy = areaScreen.trueHeight;
    else if (dy < 0) dy = 0;
    flushCursor();
    posY = areaScreen.trueHeight - dy;
    curPoint.setxy(dx, posY);
    cursor->areaCursor.left = dx + cursor->corX;
    cursor->areaCursor.top = posY + cursor->corY;
    paintArea(&cursor->areaCursor);
}

void Gcomposer::changeResolution(int width, int height) {
    if (buffer)
        delete(buffer);
    buffer = new int[(width + 1)*(height + 1)];
    changeModeAdvance(1, width, height);
    screen = Grect(0, 0, width, height);
    areaScreen.area = buffer;
    areaScreen.trueLeft = 0;
    areaScreen.trueTop = 0;
    areaScreen.trueWidth = width;
    areaScreen.width = width;
    areaScreen.height = height;
    areaScreen.trueHeight = height;
    paintArea(&areaScreen);
}

void Gcomposer::drawWindow(Sframe *frame) {
    Grect drawRect = frame->drawRect;
    int *src = frame->painter->canvas->getGraphicsBitmap();
    int left = frame->drawRect.left();
    int top = frame->drawRect.top();
    activeFrame = frame;
    if (drawRect.intersects(screen)) { //Evitar estar fuera del area de la pantalla
        drawRect &= screen; //Parte Visible
        if (left < 0) {
            src += (-left);
            left = 0;
        }
        if (top < 0) {
            src += (-top) * frame->frameRect.width();
            top = 0;
        }
    }
    int *dest = areaScreen.area + left + top * areaScreen.trueWidth;
    copyBuffers(src, dest, drawRect.height(), drawRect.width(), frame->frameRect.width());
    drawWindowsTop(drawRect); //Ventanas TOP
}

void Gcomposer::drawRectIntersect(Grect& rect) {
    if (rect.intersects(screen))
        rect &= screen;
    else return;
    Sframe *frame;
    for (int i = 0; i < frames->count(); i++) { //Se pintan las ventanas que se intersectan   
        frame = (Sframe*) frames->getItem(i);
        if (frame != activeFrame) {
            if (frame->drawRect.intersects(rect)) {
                Grect draw = frame->drawRect;
                draw &= rect;
                int *dest = areaScreen.area + draw.left() + draw.top() * areaScreen.trueWidth;
                int *src = frame->painter->canvas->getGraphicsBitmap() + (draw.y() - frame->drawRect.y()) * frame->frameRect.width() + (draw.x() - frame->drawRect.x());
                copyBuffers(src, dest, draw.height(), draw.width(), frame->frameRect.width());
            }
        }
    }
    flushVideo(&rect);
}

void Gcomposer::drawWindowsTop(Grect &rect) {
    Sframe *frame;
    for (int i = frames->indexOf(activeFrame) + 1; i < frames->count(); i++) {
        frame = (Sframe*) frames->getItem(i);
        if (frame->drawRect.intersects(rect)) {
            Grect draw = frame->drawRect;
            draw &= rect;
            int *dest = areaScreen.area + draw.left() + draw.top() * areaScreen.trueWidth;
            int *src = frame->painter->canvas->getGraphicsBitmap() + (draw.y() - frame->drawRect.y()) * frame->frameRect.width() + (draw.x() - frame->drawRect.x());
            copyBuffers(src, dest, draw.height(), draw.width(), frame->frameRect.width());
        }
    }
    if (rect.intersects(screen))
        rect &= screen;
    flushVideo(&rect);
}

void Gcomposer::changeCursor(eCursorType acursorType) {
    if (cursorType == acursorType) return;
    cursorType = acursorType;
    flushCursor();
    cursor = cursors[acursorType];
    cursor->areaCursor.left = dx + cursor->corX;
    cursor->areaCursor.top = posY + cursor->corY;
    paintArea(&cursor->areaCursor);
}

void Gcomposer::drawRectDirty(Sframe* frame, Grect rect) {
    activeFrame = frame;
    int *src = frame->painter->canvas->getGraphicsBitmap() + rect.top() * frame->frameRect.width() + rect.left();
    rect.move(frame->drawRect);
    int *dest = areaScreen.area + rect.left() + rect.top() * areaScreen.trueWidth;
    copyBuffers(src, dest, rect.height(), rect.width(), frame->frameRect.width());
    drawWindowsTop(rect);
}

void Gcomposer::copyBuffers(int* src, int* dest, int height, int width, int srcWidth) {
    for (int j = 0; j < height; j++) {
        fastCopy((u32*) dest, (u32*) src, (u32) width);
        src += srcWidth;
        dest += areaScreen.trueWidth;
    }
}

void Gcomposer::flushVideo(Grect* dirtyRect) {
    areaScreen.left = dirtyRect->left();
    areaScreen.top = dirtyRect->top();
    areaScreen.trueLeft = areaScreen.left;
    areaScreen.trueTop = areaScreen.top;
    areaScreen.width = dirtyRect->width();
    areaScreen.height = dirtyRect->height();
    paintArea(&areaScreen);
    if (dirtyRect->constains(curPoint))
        paintArea(&cursor->areaCursor);
}

void Gcomposer::flushCursor() {
    areaScreen.left = cursor->areaCursor.left;
    areaScreen.top = cursor->areaCursor.top;
    areaScreen.trueLeft = cursor->areaCursor.left;
    areaScreen.trueTop = cursor->areaCursor.top;
    areaScreen.width = cursor->areaCursor.width;
    areaScreen.height = cursor->areaCursor.height;
    paintArea(&areaScreen);
}

Gpoint Gcomposer::cursorPosition() {
    return curPoint;
}