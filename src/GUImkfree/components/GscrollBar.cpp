#include "GscrollBar.h"
#include "../../library/includes/stdio.h"

void onClickDownScrollBar(GgraphicComponent *sender, int x, int y) {
    ((GscrollBar*) sender)->clickPressDown(x, y);
}

void onClickUpScrollBar(GgraphicComponent *sender, int x, int y) {
    ((GscrollBar*) sender)->clickPressUp(x, y);
}

void onMouseMoveUpScrollBar(GgraphicComponent *sender, int x, int y) {
    ((GscrollBar*) sender)->mouseMove(x, y);
}

void onPaintScrollBar(GgraphicComponent *sender, Grect &rect) {
    ((GscrollBar*) sender)->draw(rect);
}

void onResizeScrollBar(GgraphicComponent *sender){
     ((GscrollBar*) sender)->resize();
}
GscrollBar::GscrollBar(int x, int y, int width, int height, scrollBarKind kind, Gcomponent *parent) : Gcomponent(x, y, width, height, parent) {
    color = 0x84351a;
    max = 0;
    inc = 1;
    min = 0;
    scrollPosition = 0;
    onScrollBy = 0;
    this->kind = kind;
    clickedCursor = false;
    font = new Gfont("a/kernel/uses/Marlett.fnt");
    onClickDown = onClickDownScrollBar;
    onClickUp = onClickUpScrollBar;
    onMouseMove = onMouseMoveUpScrollBar;
    onPaint = onPaintScrollBar;
    if (kind == kindVertical) {
        bup = new GgraphicsBitmap(font, 0xd4d4d4, 0x0, '5');
        bdown = new GgraphicsBitmap(font, 0xd4d4d4, 0x0, '6');
    } else {
        bup = new GgraphicsBitmap(font, 0xd4d4d4, 0x0, '3');
        bdown = new GgraphicsBitmap(font, 0xd4d4d4, 0x0, '4');
    }
    onResize = onResizeScrollBar;
    resize();
}

GscrollBar::~GscrollBar() {

}

void GscrollBar::draw(Grect &rect) {
    canvas->cutRect = rect;
    canvas->pointRect = position;
    canvas->drawRect(Grect(0, 0, realRect.width(), realRect.height()), 0xd4d4d4);
    canvas->borderColor(1, realRect, 0x00a4a4a4);
    canvas->drawBitmap(bup, *bup);
    canvas->drawBitmap(bdown, *bdown);
    canvas->drawRect(cursor1, 0x009e9e9e);
}

void GscrollBar::setmax(int amax) {
    max = amax;
    //if (componentPosition == cpFixed)
    resize();
}

void GscrollBar::clickPressDown(int x, int y) {
    int ax = x - drawRect.x();
    int ay = y - drawRect.y();
    canvas->cutRect = drawRect;
    if (bup1.constains(ax, ay)) {
        Grect delta = cursor1;
        delta.move(-paso, -paso);
        scrollForce(delta, cursor1);
    } else if (bdown1.constains(ax, ay)) {
        Grect delta = cursor1;
        delta.move(paso, paso);
        scrollForce(delta, cursor1);
    } else if (cursor1.constains(ax, ay)) {
        clickedCursor = true;
        posClick = Gpoint(ax, ay);
    } else if (bscroll1.constains(ax, ay)) {
        if (kind == kindVertical) {
            if (ay > cursor1.bottom())
                posClick = Gpoint(0, cursor1.bottom());
            else
                posClick = Gpoint(0, cursor1.y());
        } else {
            if (ax > cursor1.right())
                posClick = Gpoint(cursor1.right(), 0);
            else
                posClick = Gpoint(cursor1.x(), 0);
        }
        scrollForce(Gpoint(ax, ay), posClick);
    }
}

void GscrollBar::clickPressUp(int x, int y) {
    x = x - drawRect.x();
    y = y - drawRect.y();
    if (bup1.constains(x, y)) {
        refresh(bup1);
    } else if (bdown1.constains(x, y)) {
        refresh(bdown1);
    }
    clickedCursor = false;
}

void GscrollBar::mouseMove(int x, int y) {
    if (!(clickedCursor)) return;
    x = x - drawRect.x();
    y = y - drawRect.y();
    scrollForce(Gpoint(x, y), posClick);
    if (kind == kindVertical)
        posClick.sety(y);
    else
        posClick.setx(x);
}

void GscrollBar::scrolled(Gpoint newPosition, Gpoint actualPosition) {
    int deltaClick;
    if (kind == kindVertical)
        deltaClick = newPosition.y() - actualPosition.y();
    else
        deltaClick = newPosition.x() - actualPosition.x();
    if (deltaClick == 0) return;

    if (kind == kindVertical) {
        if (!(cursor1.bottom() + deltaClick < bscroll1.bottom())) {
            cursor1.sety(bscroll1.bottom() - cursor1.height() + 1);
            scrollPosition = max;
        } else if (!(cursor1.y() + deltaClick > bscroll1.y())) {
            cursor1.sety(bscroll1.y());
            scrollPosition = 0;
        } else {
            cursor1.move(0, deltaClick);
            scrollPosition = float((cursor1.y() - bscroll1.y()) * paso);
        }
    } else {
        if (!(cursor1.right() + deltaClick < bscroll1.right())) {
            cursor1.setx(bscroll1.right() - cursor1.width() + 1);
            scrollPosition = max;
        } else if (!(cursor1.x() + deltaClick > bscroll1.x())) {
            cursor1.setx(bscroll1.x());
            scrollPosition = 0;
        } else {
            cursor1.move(deltaClick, 0);
            scrollPosition = float((cursor1.x() - bscroll1.x()) * paso);
        }
    }

}


void GscrollBar::scrollForce(Gpoint newPosition, Gpoint actualPosition) {
    scrolled(newPosition, actualPosition);
    refresh();
    if (onScrollBy) onScrollBy(parent, scrollPosition);
}

void GscrollBar::resize() {
    if (kind == kindVertical) {
        int realWidth = realRect.width();
        bup1 = Grect(1, 1, realWidth - 2, realWidth - 2);
        bup->alignCenter(bup1);
        bdown1 = Grect(1, realRect.height() - realWidth + 1, realWidth - 2, realWidth - 2);
        bdown->alignCenter(bdown1);
        bscroll1 = Grect(1, realWidth - 1, realWidth - 2, realRect.height() - realWidth * 2 + 2);
        int sizeCursor = (bscroll1.height() - max/inc);
        if (sizeCursor < 10) sizeCursor = 10;
        cursor1 = Grect(2, realWidth - 1, realWidth - 4, sizeCursor);
        paso = float(max) / (bscroll1.height() - cursor1.height());
        cursor1.move(0,scrollPosition/paso);
    } else {
        int realHeight = realRect.height();
        bup1 = Grect(1, 1, realHeight - 2, realHeight - 2);
        bup->alignCenter(bup1);
        bdown1 = Grect(realRect.width() - realHeight + 1, 1, realHeight - 2, realHeight - 2);
        bdown->alignCenter(bdown1);
        bscroll1 = Grect(realHeight - 1, 1, realRect.width() - realHeight * 2 + 2, realHeight - 2);
        int sizeCursor = (bscroll1.width() - max/inc);
        if (sizeCursor < 10) sizeCursor = 10;
        cursor1 = Grect(realHeight - 1, 2, sizeCursor, realHeight - 4);
        Gpoint delta = cursor1.traslate(scrollPosition/paso,0);
        paso = float(max) / (bscroll1.width() - cursor1.width());
        
        //cursor1.move(scrollPosition/paso,0);
        scrolled(delta,cursor1);
      
    }
 
}