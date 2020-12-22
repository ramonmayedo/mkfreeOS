#include "window.h"
#include "windowserver.h"
#include "../../library/includes/stdio.h"
#include "../basics/png.h"

extern SwindowServer * windowServer;

void onClickItemClose(Sframe *sender) {
    ((Swindow*) sender)->close();
}

void onClickItemMaximized(Sframe *sender) {
    ((Swindow*) sender)->maximized();
}

void onClickRestore(Sframe *sender) {
    ((Swindow*) sender)->restore();
}

void onClickMinimized(Sframe *sender) {
    ((Swindow*) sender)->minimized();
}

Swindow::Swindow(Grect rect) : Sframe(rect, Gsize(800, 600)) {
    width = rect.width();
    height = rect.height();
    caption = "New Windows";
    font = new Gfont();
    onMoveActive = false;
    drawTitleBar = true;
    beforeSize = rect;
    windowStyle = wsNormal;
    windowState = wtNormal;
    resizeDirection == wpNone;
    frameType = ftWindow;
    loadStyleDefault();
    menuTitle = new Smenu(Grect(0, 0, 100, 250), this);
    ico = new Gfont("a/kernel/uses/Marlett.fnt");
    SmenuItem *menuItem = new SmenuItem("Restaurar", 0);
    menuItem->onClickItem = onClickRestore;
    menuItem->bitmap = new GgraphicsBitmap(ico, StyleWindow.menuBackgroundColor, StyleWindow.menuFontColor, '2');
    menuTitle->addItem(menuItem);
    menuItem = new SmenuItem("Minimizar", 2);
    menuItem->onClickItem = onClickMinimized;
    menuItem->bitmap = new GgraphicsBitmap(ico, StyleWindow.menuBackgroundColor, StyleWindow.menuFontColor, '0');
    menuTitle->addItem(menuItem);
    menuItem = new SmenuItem("Maximizar", 3);
    menuItem->onClickItem = onClickItemMaximized;
    menuItem->bitmap = new GgraphicsBitmap(ico, StyleWindow.menuBackgroundColor, StyleWindow.menuFontColor, '1');
    menuTitle->addItem(menuItem);
    menuItem = new SmenuItem("Close", 4);
    menuItem->onClickItem = onClickItemClose;
    menuItem->bitmap = new GgraphicsBitmap(ico, StyleWindow.menuBackgroundColor, StyleWindow.menuFontColor, 'r');
    menuTitle->addItem(menuItem);
    menuTitle->repaint();
    icoApp = 0;
    Grect button(0, 2, 20, 20);
    btnClose = new GgraphicsBitmap(button, ico, StyleWindow.toolButtonColor, StyleWindow.buttonFontColor, 'r');
    btnMaximized = new GgraphicsBitmap(button, ico, StyleWindow.toolButtonColor, StyleWindow.buttonMaximizedColor, '1');
    btnMinimized = new GgraphicsBitmap(button, ico, StyleWindow.toolButtonColor, StyleWindow.buttonMinimizedColor, '0');
    btnRestore = new GgraphicsBitmap(button, ico, StyleWindow.toolButtonColor, StyleWindow.buttonMaximizedColor, '2');
}

Swindow::~Swindow() {
    delete(painter);
    delete(menuTitle);
    delete(btnClose);
    delete(btnMaximized);
    delete(btnMinimized);
    delete(btnRestore);
}

void Swindow::loadStyleDefault() {
    StyleWindow.windowBorderColor = 0x00a4a4a4;
    StyleWindow.toolButtonColor = 0x00c7c7c7; //0x00434343;
    StyleWindow.buttonFontColor = 0x00df0d02;
    StyleWindow.buttonMinimizedColor = 0x0001742f;
    StyleWindow.buttonMaximizedColor = 0x00000080;
    StyleWindow.menuBackgroundColor = 0x00e3e3e3;
    StyleWindow.menuFontColor = 0x00;
}

void Swindow::onDrawWindows() {
    if (drawTitleBar) {
        painter->cutRect = frameRect;
        titleRect = Grect(0, 0, width, 24);
        painter->drawRectGradient(titleRect, 0x00545454, 0x00FFFFFF);
        if (icoApp){
          icoAppRect.alignCenterVertical(titleRect);
          painter->drawBitmapAlpha(icoApp, icoAppRect, 0, 0);
        }
        Grect text(24, 0, width - 60, font->height);
        text.alignCenterVertical(titleRect);
        painter->drawText(text, 5, font, caption.c_str(), 0x0, talignNone);
        painter->borderColor(1, drawRect, StyleWindow.windowBorderColor);
        toolButton = Grect(width - 80, 1, 79, 22);
        painter->drawRect(toolButton, StyleWindow.toolButtonColor);
        btnClose->setx(width - 30);
        painter->drawBitmap(btnClose, *btnClose, 0, 0);
        btnMaximized->setx(width - 52);
        btnRestore->setx(width - 52);

        if (windowState == wtMaximized)
            painter->drawBitmap(btnRestore, *btnRestore, 0, 0);
        else
            painter->drawBitmap(btnMaximized, *btnMaximized, 0, 0);
        btnMinimized->setx(width - 74);
        painter->drawBitmap(btnMinimized, *btnMinimized, 0, 0);
    }
}

void Swindow::setWindowsId(int id) {
    wid = id;
}

void Swindow::onMouseMove(Gpoint point) {
    eResizeDirection aresizeDirection[3][3] = {
        {wpTopLeft, wpTop, wpTopRight},
        {wpLeft, wpNone, wpRight},
        {wpBottomLeft, wpBottom, wpBottomRight},
    };
    eCursorType cursorType[3][3] = {
        {ctResizeDiagonalLeft, ctResizeVertical, ctResizeDiagonalRight},
        {ctResizeHorizontal, ctArrow, ctResizeHorizontal},
        {ctResizeDiagonalRight, ctResizeVertical, ctResizeDiagonalLeft},
    };
    if (drawTitleBar == false) return;
    Gpoint title = point;
    title.move(-drawRect.x(), -drawRect.y());
    int x = 1, y = 1;
    if (title.x() < 5) x = 0;
    else if (title.x() > width - 5) x = 2;
    if (title.y() < 5) y = 0;
    else if (title.y() > height - 5) y = 2;
    resizeDirection = aresizeDirection[y][x];
    windowServer->composer->changeCursor(cursorType[y][x]);
}

void Swindow::onClickDown(eMauseButton mauseButton, Gpoint point) {
    Gpoint title = point;
    title.move(-drawRect.x(), -drawRect.y());
    if (drawTitleBar) {
        if (mauseButton == mbLeft) {
            if (resizeDirection == wpNone) {
                if (btnMinimized->constains(title)) {
                    minimized();
                } else if (btnMaximized->constains(title)) {
                    if (windowState == wtNormal)
                        maximized();
                    else restore();
                } else if (btnClose->constains(title)) {
                    close();
                } else if (titleRect.constains(title)) {
                    onMoveActive = true;
                    movePosition = point;
                }
            } else {
                onMoveActive = true;
                movePosition = point;
            }
        } else if (mauseButton == mbRight && resizeDirection == wpNone) {
            if (titleRect.constains(title))
                windowServer->apendMenu(menuTitle);
        }
    }
}

void Swindow::onClickUp(Gpoint point) {
    onMoveActive = false;
}

void Swindow::resizeWindow(Grect rect) {
    if (width == rect.width() && height == rect.height()) {
        drawRect = rect;
        if (windowState == wtMaximized)
            painter->drawBitmap(btnRestore, *btnRestore, 0, 0);
        else
            painter->drawBitmap(btnMaximized, *btnMaximized, 0, 0);
    } else {
        width = rect.width();
        height = rect.height();
        frameRect.setwidth(width);
        frameRect.setheight(height);
        drawRect = rect;
        painter->canvas->resizeBitmap(width, height);
    }
    windowServer->resizeWindow(this);
}

void Swindow::maximized() {
    beforeSize = drawRect;
    windowState = wtMaximized;
    resizeWindow(windowServer->maximizedRect);
}

void Swindow::restore() {
    Grect ham[4];
    windowState = wtNormal;
    resizeWindow(beforeSize);
    int countRect = beforeSize.getRectsIntersects(windowServer->maximizedRect, ham);
    for (int i = 0; i < countRect; i++)
        windowServer->composer->drawRectIntersect(ham[i]);
}

void Swindow::minimized() {
    windowState = wtMinimized;
    windowServer->minimizedWindow(this);
}

void Swindow::close() {
    windowServer->closeWindows(this);
    delete(this);
}

void Swindow::loadIconApp(char* path) {
    icoApp = Gpng().loadPNG(path);
    icoAppRect = *icoApp;
    icoAppRect.setx(5);
}

bool Swindow::changePosition(Gpoint point) {
    traslate = point - movePosition;
    movePosition = point;
    int dx = traslate.x();
    int dy = traslate.y();
    if (dx == 0 && dy == 0) return false;
    if (windowState == wtMaximized) {
        windowState = wtNormal;
        painter->drawBitmap(btnMaximized, *btnMaximized, 0, 0);
    }

    Grect rectX, rectY;
    int newWidth = 0, newHeight = 0;
    if (dx) {
        rectX = drawRect;
        if (dx < 0) {
            rectX.move(rectX.width() + dx, 0);
            rectX.setwidth(-dx);
        } else
            rectX.setwidth(dx);
    }
    if (dy) {
        rectY = drawRect;
        if (dy < 0) {
            rectY.move(0, rectY.height() + dy);
            rectY.setheight(-dy);
        } else
            rectY.setheight(dy);
    }
    switch (resizeDirection) {
        case wpNone:
        {
            drawRect.move(traslate); //Se traslada la ventana a la nueva posicion
            windowServer->composer->drawWindow(this); //Se pinta
            if (dx)
                windowServer->composer->drawRectIntersect(rectX); //Se pinta las recta en x de cambio
            if (dy)
                windowServer->composer->drawRectIntersect(rectY); //Se pinta la rect aen y de cambio
            return true;
        };
        case wpTop:
        {
            newHeight = -dy;
            drawRect.move(0, dy);
            if (dy > 0) //Si se reduce yse pinta la reduccion
                windowServer->composer->drawRectIntersect(rectY); //Se pinta las recta en x de cambio
            break;
        }
        case wpBottom:
        {
            newHeight = dy;
            if (dy < 0) //Si se reduce se pinta la reduccion
                windowServer->composer->drawRectIntersect(rectY); //Se pinta las recta en x de cambio          
            break;
        }
        case wpLeft:
        {
            newWidth = -dx;
            drawRect.move(dx, 0);
            if (dx > 0) //Si se reduce se pinta la reduccion
                windowServer->composer->drawRectIntersect(rectX); //Se pinta las recta en x de cambio
            break;
        }
        case wpRight:
        {
            newWidth = dx;
            if (dx < 0) //Si se reduce se pinta la reduccion
                windowServer->composer->drawRectIntersect(rectX); //Se pinta las recta en x de cambio
            break;
        }
        case wpTopLeft:
        {
            newHeight = -dy;
            newWidth = -dx;
            drawRect.move(dx, dy);
            if (dy > 0) //Si se reduce se pinta la reduccion
                windowServer->composer->drawRectIntersect(rectY); //Se pinta las recta en x de cambio
            if (dx > 0) //Si se reduce se pinta la reduccion
                windowServer->composer->drawRectIntersect(rectX); //Se pinta las recta en x de cambio
            break;
        }
        case wpTopRight:
        {
            newHeight = -dy;
            newWidth = dx;
            drawRect.move(0, dy);
            if (dy > 0) //Si se reduce se pinta la reduccion
                windowServer->composer->drawRectIntersect(rectY); //Se pinta las recta en x de cambio     
            if (dx < 0) //Si se reduce se pinta la reduccion
                windowServer->composer->drawRectIntersect(rectX); //Se pinta las recta en x de cambio
            break;
        }
        case wpBottomLeft:
        {
            newHeight = dy;
            newWidth = -dx;
            drawRect.move(dx, 0);
            if (dy < 0) //Si se reduce se pinta la reduccion
                windowServer->composer->drawRectIntersect(rectY); //Se pinta las recta en x de cambio  
            if (dx > 0) //Si se reduce se pinta la reduccion
                windowServer->composer->drawRectIntersect(rectX); //Se pinta las recta en x de cambio
            break;
        }
        case wpBottomRight:
        {
            newHeight = dy;
            newWidth = dx;
            if (dy < 0) //Si se reduce se pinta la reduccion
                windowServer->composer->drawRectIntersect(rectY); //Se pinta las recta en x de cambio    
            if (dx < 0) //Si se reduce se pinta la reduccion
                windowServer->composer->drawRectIntersect(rectX); //Se pinta las recta en x de cambio
            break;
        }
        default: return false;
    };
    int mWidth = newWidth + width;
    int mHeight = newHeight + height;
    int maximizedHeight = windowServer->maximizedRect.height();
    int maximizedWidth = windowServer->maximizedRect.width();
    mWidth = (mWidth < 100 ? 100 : (mWidth > maximizedWidth ? maximizedWidth : mWidth));
    mHeight = (mHeight < 30 ? 30 : (mHeight > maximizedHeight ? maximizedHeight : mHeight));
    drawRect.setwidth(mWidth);
    drawRect.setheight(mHeight);
    resizeWindow(drawRect);
}