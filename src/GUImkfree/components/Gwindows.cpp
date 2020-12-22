#include "Gwindows.h"
#include "canvasPainter.h"
#include "../client/Gclient.h"
#include "../../library/includes/stdio.h"
extern Gclient *application;

void onPaintWindow(GgraphicComponent *sender, Grect &rect) {
    ((Gwindow*) sender)->draw();
}

Gwindow::Gwindow(int x, int y, int width, int height, Gcomponent *aparent) : Gcomponent(x, y, width, height, aparent) {
    backCanvas = new GcanvasPainter(new GgraphicsBitmap(0, 0, 0));
    frontCanvas = backCanvas;
    canvas = frontCanvas;
    drawRect = realRect;
    childRect = realRect;
    childRect.setxy(0,0);
    childRect.reducedN(-2,-2);
   // controlType = ctWindow;
    onPaint = onPaintWindow;
    window = this;
    controlFocused = this;
    focus = true;
    doubleBuffer = false;
}

Grect* Gwindow::getWindowsRect() {
    return &drawRect;
}

void Gwindow::flush() {
    if (doubleBuffer)
        backCanvas->drawBitmap(frontCanvas->canvas, *frontCanvas->canvas, 0, 0);
    application->refreshRect(this, drawRect);
}

void Gwindow::flushRect(Grect &rect) {
    if (doubleBuffer)
        backCanvas->drawBitmap(frontCanvas->canvas, rect, rect.x(), rect.y());
    application->refreshRect(this, rect);
}

void Gwindow::draw() {
    frontCanvas->backgroundColor(0x00e9e9e9);
}

void Gwindow::setdoubleBuffer(bool active) {
    doubleBuffer = active;
   /* if (doubleBuffer)
        frontCanvas = canvas;
    else*/
        frontCanvas = backCanvas;
}

void Gwindow::setBackCanvas(int *buffer) {
   *(Grect*)backCanvas->canvas = drawRect;
    backCanvas->canvas->setGraphicsBitmap(buffer);
}

void Gwindow::resize(int width, int height) {
    drawRect.setwidth(width);
    drawRect.setheight(height);
    childRect.setwidth(width - 2);
    childRect.setheight(height - 2);
    frontCanvas->canvas->resizeBitmap(width, height);
    frontCanvas->backgroundColor(0x00e9e9e9);
    resizeComponent();
    if (doubleBuffer)
        backCanvas->copyBitmap(frontCanvas->canvas);
}

void Gwindow::onCreate() {
     if (doubleBuffer)
        backCanvas->copyBitmap(frontCanvas->canvas);
}

Gwindow::~Gwindow() {

}