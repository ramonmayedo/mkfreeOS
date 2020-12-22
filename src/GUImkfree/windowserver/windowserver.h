#ifndef WINDOWSERVER_H
#define WINDOWSERVER_H
#include "../basics/list.h"
#include "window.h"
#include "composer.h"
#include "desktop.h"
#include "maintoolbar.h"
#include "menu.h"
#include "clientconnection.h"


enum eCommandServer {
    csCreateConnection, csCreateWindows, csPaintWindows,
    csActiveWindows, csMinimizedWindows, csRepaintZone,
    csRestoreWindows, csSetPIDServer, csCloseWindows, csSetSharedMemory,
    csPaintRect, esClickDown, esClickUp, esKeyDown, esMauseMove, esDoubleClick,
    csShowWindows, csResizeWindow, csSetCaption, csSetIconPath
};

class SwindowServer {
public:
    SwindowServer();
    int createWindow(Swindow *window);
    void activateWindow(Sframe *window);
    void activateWindow(int wid);
    void minimizedWindow(Swindow *window);
    void minimizedWindow(int wid);
    void closeWindows(Swindow *window);
    void resizeWindow(Swindow *window);
    Swindow *getWindow(int wid);
    void onActivate();
    void eventOcurred(eCommandServer event, eMauseButton mauseButton,  Gpoint point, int value);
    void eventOcurred(int key);
    void apendMenu(Smenu *menu);
    void closeMenu();
    void executeCommnd(int command, int pidClient, int parameter1, int parameter2);
    Sframe *getWindows(Gpoint &point);
    Sframe* getWindowsOnly(Gpoint& point); 
    Gcomposer *composer;
    SclientConnection *clientConnection;
    Sframe *activeWindow, *activeMenu;
    Grect maximizedRect;
    ~SwindowServer();
private:
    Clist *windows, *minWindows, *menus;
    Sframe *nullWindow;
    int wid;
    int indexTop;
    SmainToolBar *mainToolBar;
    Sdesktop *destk;
};

#endif
