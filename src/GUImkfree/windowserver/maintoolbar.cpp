#include "maintoolbar.h"
#include "toolapp.h"
#include "windowserver.h"


 extern SwindowServer * windowServer;

int gwid;
bool active;

void onClickToolButtonDown(StoolButton *sender) {
    if (sender->bid == gwid) {
        if (active) active = false;
        else active = true;
    } else {
        active = true;
        gwid = sender->bid;
    }

    if (active)
        windowServer->activateWindow(sender->bid);
    else
        windowServer->minimizedWindow(sender->bid);

}

SmainToolBar::SmainToolBar(Grect rect) : Sframe(rect) {
    painter->backgroundColor(0x00c0c0c0);
    buttonStart = Grect(0, 0, 80, rect.height());
    frameType = ftToolBar;
    buttonStart.alignCenter(frameRect);
    toolApp = new StoolApp(this, Grect(0, 0, buttonStart.x(), rect.height()));
    toolApp->repaint();
    repaint();
    gwid = 0;
   /* menuApp = new Smenu(Grect(0, 0, 100, 250),this);
    menuApp->addItem("Propiedades", 0, tmItem);
    menuApp->addItem("Estilo", 0, tmItem);
    menuApp->repaint();*/
}

void SmainToolBar::onClickDown(Gpoint point) {
    point -= drawRect;
    if (toolApp->constains(point))
        toolApp->onClickDown(point);
     else {
    //    windowServer->apendMenu(menuApp);
     }
}

void SmainToolBar::repaint() {
    painter->drawRect(buttonStart, 0x004f4f4f);
}

void SmainToolBar::addApp(int wid, GgraphicsBitmap *bitmap) {
    StoolButton *toolButton = new StoolButton(Grect(0, 0, 24, 24), bitmap);
    toolButton->bid = wid;
    toolButton->onPressDown = onClickToolButtonDown;
    toolApp->addToolButton(toolButton);
    gwid = wid;
    active = true;
}

void SmainToolBar::activeApp(int wid) {
    if (toolApp->bottonSelected) {
        if (toolApp->bottonSelected->bid != wid) {
            toolApp->selectButton(wid);
            gwid = wid;
            active = true;
        }
    } else
        toolApp->selectButton(wid);
}

void SmainToolBar::closeApp(int wid) {
    toolApp->deleteButton(wid);
}

void SmainToolBar::changeIcoApp(int wid, GgraphicsBitmap *bitmap) {
     
}

void SmainToolBar::deselctApp() {
    //toolApp->deselectButton();
    active = false;
}

SmainToolBar::~SmainToolBar() {

}