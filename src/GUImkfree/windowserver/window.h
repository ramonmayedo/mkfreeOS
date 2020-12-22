
#ifndef WINDOW_H
#define WINDOW_H
#include "../basics/cstring.h"
#include "frame.h"
#include "menu.h"
#define  maxSpaceResize  3

enum eWindowStyle {
    wsNormal, wsBack, wsTop
};

enum eWindowState {
    wtNormal, wtMinimized, wtMaximized
};

enum eMauseButton {
    mbLeft = 1, mbRight = 2, mbMiddle = 4
};

enum eResizeDirection {
    wpNone, wpLeft, wpRight, wpTop, wpBottom,
    wpTopLeft, wpTopRight, wpBottomLeft, wpBottomRight
};

struct sStyleWindow {
    Gcolor windowBorderColor;
    Gcolor windowBackGroundColor;
    Gcolor buttonCloseColor;
    Gcolor buttonFontColor;
    Gcolor buttonMinimizedColor;
    Gcolor buttonMaximizedColor;
    Gcolor titleInactiveBackgroundColor;
    Gcolor titleActiveBackgroundColor;
    Gcolor menuBackgroundColor;
    Gcolor menuFontColor;
    Gcolor menuBorderColor;
    Gcolor toolButtonColor;
};

class Swindow : public Sframe {
public:
    Gpoint traslate;
    eWindowStyle windowStyle;
    eWindowState windowState;
    bool onMoveActive;
    Smenu *menuTitle;
    int width, height;
    int wid;
    int pid;
    Swindow(Grect rect);
    void onDrawWindows();
    void loadStyleDefault();
    void setWindowsId(int id);
    void onClickDown(eMauseButton mauseButton, Gpoint point);
    void onMouseMove(Gpoint point);
    void onClickUp(Gpoint point);
    void resizeWindow(Grect rect);
    bool changePosition(Gpoint point);
    void loadIconApp(char *path);
    void maximized();
    void restore();
    void minimized();
    void close();
    ~Swindow();
    string caption;
    GgraphicsBitmap *icoApp;
private:
    Grect titleRect, beforeSize;
    Grect toolButton;
    GgraphicsBitmap *btnClose, *btnMaximized, *btnMinimized, *btnRestore;
    Grect icoAppRect;
    Gpoint movePosition;
    Gfont *font, *ico;
    eResizeDirection resizeDirection;
    bool drawTitleBar;
    sStyleWindow StyleWindow;
};

#endif