#include "Gbutton.h"
#include "../../library/includes/stdio.h"

enum eButtonStyle {
    styGrey, styBlue, styGreen
};

enum eButtonState {
    bsNormal, bsClicked
};

void onClickDownButton(GgraphicComponent *sender, int x, int y) {
   ((Gbutton*) sender)->clickDown();
}

void onClickUpButton(GgraphicComponent *sender, int x, int y) {
   ((Gbutton*) sender)->clickUp();
}

void onDrawButton(GgraphicComponent *sender,Grect &rect) {
    ((Gbutton*) sender)->draw(rect);
}

Gbutton::Gbutton(int x, int y, int width, int height, Gcomponent *parent) : Gcomponent(x, y, width, height, parent) {
    onClickDown = onClickDownButton;
    onClickUp = onClickUpButton;
    onPaint = onDrawButton;
    style = 0;
    bstate = bsNormal;
    //controlType = ctButton;
}

void Gbutton::clickDown() {
    bstate = bsClicked;
    refresh();
}

void Gbutton::clickUp() {
    bstate = bsNormal;
    refresh();
}

void Gbutton::draw(Grect &eventDrawRect) {
    int styles[2][3] = {
        {0x00d4d4d4, 0x00545454, 0x004f4f4f}, //BackgroundColor, focusColor, BorderColor
        {0x00ff6666, 0x00cc3333, 0x004f4f4f},
    };
    int pres;
    canvas->cutRect = eventDrawRect;
    canvas->pointRect = position;
    int width = realRect.width();
    int height = realRect.height();
    canvas->drawRect(Grect(0, 0, width, height), styles[style][0]);
    canvas->borderColor(1, realRect, styles[style][2]);
    if (bstate == bsNormal) {
        canvas->drawRect(Grect(0, height - 2, width, 1), styles[style][2]);
        canvas->drawRect(Grect(width - 2, 0, 1, height), styles[style][2]);
        pres = 0;
    } else if (bstate == bsClicked) {
        canvas->drawRect(Grect(0, 1, width, 1), styles[style][2]);
        canvas->drawRect(Grect(1, 0, 1, height), styles[style][2]);
        pres = 1;
    }
    if (caption.Lenght())
        canvas->drawText(Grect(pres, 0, width, height), 0, font, caption, 0, talignCenter);
}

Gbutton::~Gbutton() {
}