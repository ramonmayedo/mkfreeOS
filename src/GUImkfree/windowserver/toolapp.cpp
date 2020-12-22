#include "toolapp.h"
#include "windowserver.h"
#include "../../library/includes/stdio.h"

StoolButton::StoolButton(Grect rect, GgraphicsBitmap *abitmap) : Grect(rect) {
    onPressDown = 0;
    if (abitmap)
        bitmap = new GgraphicsBitmap(abitmap->width(), abitmap->height(), abitmap->getGraphicsBitmap());
     else
        bitmap = 0;
}

void StoolApp::paintButton(StoolButton* button, bool selected) {
    int color;
    if (selected) color = 0x00656565;
    else color = 0x00cacaca;
    canvas->drawRect(*(Grect*) button, color);
    if (button->bitmap)
        canvas->drawBitmapAlpha(button->bitmap, *button->bitmap, 0, 0);
    parent->refreshRect(*(Grect*) button);
}

StoolApp::StoolApp(Sframe* frame, Grect rect) : Grect(rect) {
    canvas = frame->painter;
    parent = frame;
    int height = rect.height();
    buttonLeft = Grect(0, 0, 20, height);
    buttonRight = Grect(0, 0, 20, height);
    buttonRight.alignRight(rect);
    maxToolVisible = ((rect.width() - 42) / 25);
    toolButtonRect = Grect(0, 0, maxToolVisible * 25, height);
    toolButtonRect.alignCenterHorizontal(rect);
    indexPaint = 0;
    bottonSelected = 0;
}

void StoolApp::addToolButton(StoolButton *button) {
    buttons.add(button);
    repaintButton(button);
    if (indexPaint < maxToolVisible)
        indexPaint++;
}

void StoolApp::repaint() {
    //canvas->drawRect(buttonLeft, 0x00fd5913);
    //canvas->drawRect(buttonRight, 0x00fd5913);
}

void StoolApp::repaintButton(StoolButton *button) {
    int index = buttons.indexOf(button);
    button->alignCenterVertical(toolButtonRect);
    button->move(toolButtonRect.x() + index * 25, 0);
    if (button->bitmap)
    button->bitmap->alignCenter(*button);
    if (index >= maxToolVisible) return;
    if (!bottonSelected)
        bottonSelected = button;
    paintButton(button, true);
}

void StoolApp::onClickDown(Gpoint point) {
    StoolButton *toolButton;
    for (int i = 0; i < indexPaint; i++) {
        toolButton = (StoolButton*) buttons.getItem(i);
        if (toolButton->constains(point)) {
            if (toolButton != bottonSelected) {
                paintButton(toolButton, true);
                if (bottonSelected)
                    paintButton(bottonSelected, false);
                bottonSelected = toolButton;
            }
            if (bottonSelected->onPressDown)
                bottonSelected->onPressDown(bottonSelected);
            return;
        }
    }
}

void StoolApp::selectButton(int bid) {
    StoolButton *toolButton;
    for (int i = 0; i < indexPaint; i++) {
        toolButton = (StoolButton*) buttons.getItem(i);
        if (toolButton->bid == bid) {
            paintButton(toolButton, true);
            if (bottonSelected)
                paintButton(bottonSelected, false);
            bottonSelected = toolButton;
            return;
        }
    }
}

void StoolApp::deleteButton(int bid) {
    StoolButton *toolButton;
    for (int i = 0; i < buttons.count(); i++) {
        toolButton = (StoolButton*) buttons.getItem(i);
        if (toolButton->bid == bid) {
            buttons.remove(toolButton);
            bottonSelected = 0;
            repaintAll();
            delete(toolButton);
            return;
        }
    }
}

void StoolApp::deselectButton() {
    if (bottonSelected) {
        paintButton(bottonSelected, false);
        bottonSelected = 0;
    }
}

void StoolApp::repaintAll() {
    if (buttons.count() > maxToolVisible)
        indexPaint = maxToolVisible;
    else
        indexPaint = buttons.count();
    canvas->drawRect(toolButtonRect, 0x00c0c0c0);
    StoolButton *toolButton;
    for (int i = 0; i < indexPaint; i++) {
        toolButton = (StoolButton*) buttons.getItem(i);
        toolButton->alignCenterVertical(toolButtonRect);
        toolButton->setx(toolButtonRect.x() + i * 25);
        if (toolButton->bitmap) {
            toolButton->bitmap->alignCenter(*toolButton);
            canvas->drawBitmapAlpha(toolButton->bitmap, *toolButton->bitmap, 0, 0);
        }
    }
    parent->refreshRect(toolButtonRect);
}

StoolApp::~StoolApp() {
}

StoolButton::~StoolButton() {
}
