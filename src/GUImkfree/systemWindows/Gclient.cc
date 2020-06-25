/*Copyright (C) 2019  Ramón Mayedo Morales (ramonmayedo@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "Gclient.h"
#include "../components/GlistBox.h"
#include "../components/GtextEdit.h"
#include "../components/GlineEdit.h"
#include "../components/GcolumnButton.h"
#include "../components/GlistView.h"
#include "../components/GtoolBar.h"
#include "../../library/includes/unistd.h"

Gclient::Gclient() {
    int command, pidServidor, parameter1, parameter2;
    do {
        getCommand(&command, &pidServidor, &parameter1, &parameter2);
    } while (command != cmsSetPIDServer);
    pidServer = parameter1;
    pidClient = getPid();
}

int Gclient::createWindows(Gwindows* windows) {
    this->windows = windows;
}

int Gclient::activate() {
    int command = 0, pidServidor, status, parameter2;   
    sendCommand(cmsCreateConnection, pidServer, 0, 0);  //Solicitud de Conexion con el servidor
    getCommand(&command, &pidServidor, &status, &parameter2);
    if (status == 0) { //Se acepto la conexion     
        Grect *rect = windows->graphics;  //Se crea una ventana
        writeBufferIPC(pidClient, (char*) rect, 0, sizeof (Grect));
        sendCommand(cmsCreateWindows, pidServer, 1, 1);
        getCommand(&command, &pidServidor, &status, &parameter2);
        if (status == 0) { //Se creo la ventana
            while (1) {
                getCommand(&command, &pidServidor, &status, &parameter2);
                executeCommnd(command, pidServidor, status, parameter2);
                if (command == cmsCloseWindows) return 0;
            }
        }
    }
    return 0;
}

int Gclient::executeCommnd(int command, int pidServer, int parameter1, int parameter2) {
    switch (command) {
        case cmsPaintWindows:
        {
            OnDraw(windows);
            windows->refresh();
            windows->reasignPositionsChilds();
            windows->OnCreate(windows);
            return 0;
        }
        case cmsActiveWindows:
        {
            windows->refresh();
            return 0;
        }
        case cmsCloseWindows:
        {
            return 0;
        }
        case cmeClickDown:
        {
            Gcontrol *selectedControl = windows->getControl(parameter1, parameter2);
            if (windows->controlFocused && windows->controlFocused != selectedControl) {
                windows->controlFocused->focus = false;
                onFocused(windows->controlFocused);
            }
            windows->controlFocused = selectedControl;
            selectedControl->focus = true;
            onClickDown(selectedControl, parameter1, parameter2);
            return 0;
        }
        case cmeClickUp:
        {
            Gcontrol *selectedControl = windows->getControl(parameter1, parameter2);
            onClickUp(selectedControl, parameter1, parameter2);
            return 0;
        }
        case cmeDoubleClick:
        {
            Gcontrol *selectedControl = windows->getControl(parameter1, parameter2);
            onDoubleClick(selectedControl, parameter1, parameter2);
            return 0;
        }
        case cmeKeyDown:
        {
            int key = parameter1;
            if (windows->controlFocused)
                onKeyDown(windows->controlFocused, key);
            return 0;
        }
        case cmeMauseMove:
        {
            if (windows->controlMouseMove)
                leaveMouseMove(windows->controlMouseMove);
            Gcontrol *selectedControl = windows->getControl(parameter1, parameter2);
            windows->controlMouseMove = selectedControl;
            onMouseMove(selectedControl, parameter1, parameter2);
            return 0;
        }
        case cmsRepaintZone:
        {
            Grect *newZone;
            readBufferIPC(pidClient, (char*) newZone, 0, sizeof (Grect));
            windows->repaintZone(*newZone);
            return 0;
        }
        case cmsSetSharedMemory:
        {
            bufferShared = (int*)setshmem(pidServer, parameter1, parameter2);
            return 0;
        }
        default: return 0;
    }
}

int Gclient::applicationCommand(int command) {
    sendCommand(command, 2, getPid(), 0);
    return 0;
}

void Gclient::onClickDown(Gcontrol* sender, int x, int y) {
    if (sender->classNameIs("Gbutton")) {
        ((Gbutton*) sender)->clickPressDown(sender, x, y);
    } else if (sender->classNameIs("GlistBox")) {
        ((GlistBox*) sender)->selectItem(x, y);
    } else if (sender->classNameIs("GtextEdit")) {
        ((GtextEdit*) sender)->clickPressDown(x, y);
    } else if (sender->classNameIs("GlineEdit")) {
        ((GlineEdit*) sender)->clickPressDown(x);
    } else if (sender->classNameIs("GcolumnButton")) {
        ((GcolumnButton*) sender)->clickPressDown(sender, x, y);
    } else if (sender->classNameIs("GlistView")) {
        ((GlistView*) sender)->clickPressDown(x, y);
    }
    if (sender->onClickDown != 0) sender->onClickDown(sender, x, y);
}

void Gclient::onClickUp(Gcontrol* sender, int x, int y) {
    if (sender->classNameIs("Gbutton")) {
        ((Gbutton*) sender)->clickPressUp(sender, x, y);
    } else if (sender->classNameIs("GcolumnButton")) {
        ((GcolumnButton*) sender)->clickPressUp(sender, x, y);
    } else if (sender->classNameIs("GlineEdit")) {
        ((GlineEdit*) sender)->clickPressUp(x);
    } else if (sender->classNameIs("GtextEdit")) {
        ((GtextEdit*) sender)->clickPressUp(x, y);
    } else if (sender->classNameIs("GlistView")) {
        ((GlistView*) sender)->clickPressUp(x, y);
    }
    if (sender->onClickUp != 0) sender->onClickUp(sender, x, y);
}

void Gclient::onDoubleClick(Gcontrol* sender, int x, int y) {
    if (sender->onDoubleClick != 0) sender->onDoubleClick(sender, x, y);
}

void Gclient::onKeyDown(Gcontrol* sender, int &key) {
    if (sender->classNameIs("GlistBox")) {
        ((GlistBox*) sender)->keyPress(key);
    } else if (sender->classNameIs("GtextEdit")) {
        ((GtextEdit*) sender)->keyPress(key);
    } else if (sender->classNameIs("GlineEdit")) {
        ((GlineEdit*) sender)->keyPress(key);
    } else if (sender->classNameIs("GlistView")) {
        ((GlistView*) sender)->keyPress(key);
    }
    if (sender->onKeyPress != 0) sender->onKeyPress(sender, key);
}

void Gclient::onFocused(Gcontrol *sender) {
    if (sender->classNameIs("GlistBox")) {
        ((GlistBox*) sender)->focused();
    } else if (sender->classNameIs("GtextEdit")) {
        ((GtextEdit*) sender)->focused();
    } else if (sender->classNameIs("GlineEdit")) {
        ((GlineEdit*) sender)->focused();
    } else if (sender->classNameIs("GlistView")) {
        ((GlistView*) sender)->focused();
    }
    if (sender->onFocused != 0) sender->onFocused(sender);
}

void Gclient::leaveMouseMove(Gcontrol* sender) {
    if (sender->classNameIs("GtoolButton")) {
        ((GtoolButton*) sender)->refresh();
    }
}

void Gclient::onMouseMove(Gcontrol* sender, int x, int y) {
    if (sender->classNameIs("GtoolButton")) {
        ((GtoolButton*) sender)->mouseMove(sender, x, y);
    } else if (sender->classNameIs("GlineEdit")) {
        ((GlineEdit*) sender)->mouseMove(x);
    } else if (sender->classNameIs("GlistView")) {
        ((GlistView*) sender)->mouseMove(x, y);
    }
}

int Gclient::execApp(char* fileName, char* argv[]) {
    int pidApp = execv(fileName, argv);
    sendCommand(cmsSetPIDServer, pidApp, pidServer, 0);
}
