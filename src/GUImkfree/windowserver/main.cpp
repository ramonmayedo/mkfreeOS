
#include "../../library/includes/cprogram.h"
#include "composer.h"
#include "windowserver.h"
#include "../../library/includes/thread.h"

SwindowServer * windowServer;
void* getMouseEvent(void*) ;
void* getKeyEvent(void*) ;

int kmain(int argc, char** argv) {
    cthread(getMouseEvent);
    cthread(getKeyEvent);
    windowServer = new SwindowServer();
    windowServer->onActivate();
    return 0;
}

void* getMouseEvent(void*) {
    int dx = 0, dy = 0, dz = 0;
    eMauseButton button;
    int bx = 0, by = 0;
    bool clicked = false;
    do {
        (int) button = getMouseState(&dx, &dy, &dz);
        if (dx != bx || dy != by)
            windowServer->composer->changePositionCursor(dx, dy);
        if (clicked == true && button == 0) { //Mouse Up
            windowServer->eventOcurred(esClickUp, button, windowServer->composer->cursorPosition(), 0);
            clicked = false;
        } else if (button != 0 && clicked == false) {//Mouse Down
            windowServer->eventOcurred(esClickDown, button, windowServer->composer->cursorPosition(), 0);
            clicked = true;
        }
        if (dx != bx || dy != by) {
            windowServer->eventOcurred(esMauseMove, button, windowServer->composer->cursorPosition(), 0);
            bx = dx;
            by = dy;
        }
    } while (1);
}

void* getKeyEvent(void*) {
    do {
        char key = getKeyPress();
        windowServer->eventOcurred(key);
    } while (1);
}