#include "windowserver.h"
#include "../../library/includes/unistd.h"
#include "../../library/includes/stdio.h"

SwindowServer::SwindowServer() {
    wid = 1;
    windows = new Clist();
    minWindows = new Clist();
    menus = new Clist();
    composer = new Gcomposer(windows);
    nullWindow = new Sframe(Grect(0, 0, 1, 1));
    clientConnection = new SclientConnection(getPid());
}

int SwindowServer::createWindow(Swindow* window) {
    window->setWindowsId(wid);
    windows->insert(windows->count() - 1, window);
    wid++;
}

void SwindowServer::activateWindow(Sframe* window) {
    composer->drawWindow(window);
}

void SwindowServer::activateWindow(int wid) {
    Sframe *window;
    for (int i = 0; i < minWindows->count(); i++) {
        window = (Sframe*) minWindows->getItem(i);
        if (window->frameType == ftWindow) {
            if (((Swindow*) window)->wid == wid) {
                minWindows->remove(window);
                windows->insert(windows->count() - 1, window);
                ((Swindow*) window)->windowState = wtNormal;
                composer->drawWindow(window);
                return;
            }
        }
    }
    for (int i = 0; i < windows->count(); i++) {
        window = (Sframe*) windows->getItem(i);
        if (window->frameType == ftWindow) {
            if (((Swindow*) window)->wid == wid) {
                windows->remove(window);
                windows->insert(windows->count() - 1, window);
                composer->drawWindow(window);
                return;
            }
        }
    }
}

void SwindowServer::minimizedWindow(int wid) {
    Sframe *window;
    for (int i = 0; i < windows->count(); i++) {
        window = (Sframe*) windows->getItem(i);
        if (window->frameType == ftWindow) {
            if (((Swindow*) window)->wid == wid) {
                windows->remove(window);
                minWindows->add(window);
                composer->drawWindowsTop(window->drawRect);
                activeWindow = (Swindow*) windows->getFirst();
                return;
            }
        }
    }
}

void SwindowServer::minimizedWindow(Swindow* window) {
    windows->remove(window);
    minWindows->add(window);
    composer->drawWindowsTop(window->drawRect);
    activeWindow = (Sframe*) windows->getFirst();
    mainToolBar->deselctApp();
}

void SwindowServer::closeWindows(Swindow* window) {
    windows->remove(window);
    composer->drawWindowsTop(window->drawRect);
    activeWindow = (Sframe*) windows->getFirst();
    mainToolBar->closeApp(window->wid);
}

void SwindowServer::resizeWindow(Swindow* window) {
    sendCommand(csResizeWindow, window->pid, window->wid, (int) (window->painter->canvas->getLine(24) + 1));
    sendCommand(csResizeWindow, window->pid, window->width, window->height-24);
}

Swindow * SwindowServer::getWindow(int wid) {
    Sframe *window;
    for (int i = 0; i < windows->count(); i++) {
        window = (Sframe*) windows->getItem(i);
        if (window->frameType == ftWindow) {
            if (((Swindow*) window)->wid == wid)
                return (Swindow*)window;
        }
    }
}

void SwindowServer::onActivate() {
    composer->changeResolution(800, 600);
    destk = new Sdesktop(Grect(0, 0, 800, 600));
    destk->loadWallpaper();
    windows->add(destk);
    activateWindow(destk);

    mainToolBar = new SmainToolBar(Grect(0, 570, 800, 30));
    windows->add(mainToolBar);
    activateWindow(mainToolBar);

    maximizedRect = Grect(0, 0, 800, 570);

 /*   Swindow *window = new Swindow(Grect(25, 25, 500, 500));
    createWindow(window);
    activateWindow(window);

    window = new Swindow(Grect(50, 50, 500, 500));
    createWindow(window);
    activateWindow(window);
   //  for (int i=0;i<20;i++){
    window = new Swindow(Grect(60, 60, 400, 400));
    createWindow(window);
    activateWindow(window);
   //  }*/
    char **arg = 0;
    int pidFileExplorer = execv("a/fileExplorer.elf", arg);
    sendCommand(csSetPIDServer, pidFileExplorer, getPid(), 0);
    
    int notepad = execv("a/notepad.elf", arg);
    sendCommand(csSetPIDServer, notepad, getPid(), 0); 
    activeMenu = 0;
    int command, pidTrasmisor, parameter1, parameter2, recive;
    do {
        recive = getCommand(&command, &pidTrasmisor, &parameter1, &parameter2);
        executeCommnd(command, pidTrasmisor, parameter1, parameter2);
      } while (1);
}

void SwindowServer::eventOcurred(eCommandServer event, eMauseButton mauseButton,  Gpoint point, int value) {
    int x = point.x() - ((Swindow*) activeWindow)->drawRect.x() - 1;
    int y = point.y() - ((Swindow*) activeWindow)->drawRect.y() - 24;
    switch (event) {
        case esClickDown:
        {
            if (!activeWindow->drawRect.constains(point) || activeWindow->frameType == ftDesktop) {
                if (activeWindow->frameType == ftMenu)
                    closeMenu();
                activeWindow = getWindows(point);
                if (activeWindow->frameType == ftWindow)
                    composer->drawWindow(activeWindow);
            }

            if (activeWindow->frameType == ftWindow){
                ((Swindow*) activeWindow)->onClickDown(mauseButton, point);
                sendCommand(esClickDown, ((Swindow*)activeWindow)->pid, x, y);
            }else if (activeWindow->frameType == ftToolBar)
                ((SmainToolBar*) activeWindow)->onClickDown(point); 
            else if (activeWindow->frameType == ftMenu)
                ((Smenu*) activeWindow)->onClickDown(point); 
            break;
        }
        case esClickUp:
        {
            if (activeWindow->frameType == ftWindow) {
                ((Swindow*) activeWindow)->onClickUp(point);
                sendCommand(esClickUp, ((Swindow*) activeWindow)->pid, x, y);
            }
            break;
        }
        case esMauseMove:
        {
            if (activeWindow->frameType == ftWindow) {
                if (((Swindow*) activeWindow)->onMoveActive == true)
                    ((Swindow*) activeWindow)->changePosition(point);
                else {
                    if (((Swindow*) activeWindow)->drawRect.constains(point)){
                        ((Swindow*) activeWindow)->onMouseMove(point);
                        sendCommand(esMauseMove, ((Swindow*) activeWindow)->pid, x, y);
                    }else {
                        Sframe *noActive = getWindowsOnly(point);
                        if (noActive->frameType == ftWindow)
                            ((Swindow *)noActive)->onMouseMove(point);
                    }
                }
            } else if (activeWindow->frameType == ftMenu) {
                if (activeWindow->drawRect.constains(point))
                    ((Smenu*) activeWindow)->onMouseMove(point);
            }else {
                Sframe *noActive = getWindowsOnly(point);
                if (noActive->frameType == ftWindow)
                    ((Swindow *) noActive)->onMouseMove(point);
            }

            break;
        }
        default: break;
    };
}

void SwindowServer::eventOcurred(int key) {
    if (activeWindow->frameType == ftWindow)
        sendCommand(esKeyDown, ((Swindow*) activeWindow)->pid, key, 0);
}


Sframe* SwindowServer::getWindows(Gpoint& point) {
    Sframe *window;
    for (int i = windows->count() - 1; i >= 0; i--) {
        window = (Sframe*) windows->getItem(i);
        if (window->drawRect.constains(point)) {
            if (window->frameType == ftWindow) {
                windows->remove(window);
                windows->insert(windows->count() - 1, window);
                mainToolBar->activeApp(((Swindow*) window)->wid);
            }
            return window;
        }
    }
    return nullWindow;
}

Sframe* SwindowServer::getWindowsOnly(Gpoint& point) {
    Sframe *window;
    for (int i = windows->count() - 1; i >= 0; i--) {
        window = (Sframe*) windows->getItem(i);
        if (window->drawRect.constains(point)) {
            if (window->frameType == ftWindow)
                return window;
        }
    }
    composer->changeCursor(ctArrow);
    return nullWindow;
}


void SwindowServer::apendMenu(Smenu* menu) {
    if (activeWindow) {
        if (activeWindow != menu)
            windows->add(menu);
    }
     
    Gpoint point = composer->cursorPosition();
    int px = point.x();
    int py = point.y();
    if (py + menu->drawRect.height() > 600)
        menu->drawRect.sety(py - menu->height);
    else
        menu->drawRect.sety(py);

    if (px + menu->width > 800)
        menu->drawRect.setx(px - menu->width);
    else
        menu->drawRect.setx(px);

    activeWindow = menu;
    composer->drawWindow(menu);
}

void SwindowServer::closeMenu() {
    windows->remove(activeWindow);
    composer->drawRectIntersect(activeWindow->drawRect);
    activeWindow = destk;
}

void SwindowServer::executeCommnd(int command, int pidClient, int parameter1, int parameter2) {
    switch (command) {
        case csCreateConnection:
        {
            clientConnection->addConnection(pidClient);
            sendCommand(csCreateConnection, pidClient, 0, 0);
            break;
        }
        case csCreateWindows:
        {
            Grect *rectW = new Grect();
            if (parameter1 == 1)  //Se lee del buffer ipc
                readBufferIPC(pidClient, (char*) rectW, 0, sizeof (Grect));
            Swindow *window = new Swindow(*rectW);
            window->pid = pidClient;
            createWindow(window);
            sendCommand(csCreateConnection, pidClient, 0, window->wid);       //Conexion aceptada
            int *dirSharedToPaint = window->painter->canvas->getLine(24) + 1; //
            sendCommand(csSetSharedMemory, pidClient, (int) dirSharedToPaint, window->keyShared);
            break;
        }
        case csShowWindows:
        {
            Swindow *wind = getWindow(parameter1);
            mainToolBar->addApp(wind->wid, wind->icoApp);
            mainToolBar->activeApp(wind->wid);
            activeWindow = wind;
            wind->onDrawWindows();
            activateWindow(wind);
            break;
        }
        case csResizeWindow:
        {
            Swindow *wind = getWindow(parameter1);
            wind->onDrawWindows();
            composer->drawWindow(wind);
            break;
        }
        case csSetCaption:
        {
            Swindow *wind = getWindow(parameter1);
            char *newCaption = new char[parameter2];
            readBufferIPC(wind->pid, (char*) newCaption, 0, parameter2);
            sendCommand(csSetCaption, pidClient, 0, 0);
            wind->caption = newCaption;
            delete(newCaption);
            break;
        }
        case csSetIconPath:
        {
            Swindow *wind = getWindow(parameter1);
            char *newCaption = new char[parameter2];
            readBufferIPC(wind->pid, (char*) newCaption, 0, parameter2);
            sendCommand(csSetCaption, pidClient, 0, 0);
            wind->loadIconApp(newCaption);
            delete(newCaption);
            break;
        }
        case csPaintRect:
        {
            Grect rectW;
            if (parameter1 == 1) //Se lee del buffer ipc
                readBufferIPC(pidClient, (char*) &rectW, 0, sizeof (Grect));
            Swindow *window = getWindow(parameter2);
            rectW.move(1, 24);
            rectW &= window->frameRect;
            composer->drawRectDirty(window, rectW);
            break;
        }
    }
}

SwindowServer::~SwindowServer() {

}