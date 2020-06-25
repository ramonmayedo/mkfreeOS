/*Copyright (C) 2019  Ram�n Mayedo Morales (ramonmayedo@gmail.com)

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

#include "Gdestokp.h"
#include "../../systemWindows/Gclient.h"
#include "../../../library/includes/unistd.h"

Gdestokp::Gdestokp() {
    int command, pidServidor, parameter1, parameter2;
    do {
        getCommand(&command, &pidServidor, &parameter1, &parameter2);
    } while (command != cmsSetPIDServer);
    pidServer = parameter1;
    pidClient = getPid();
    controlFocused = 0;
    controlMouseMove = 0;
    windows = new Grect(0, 0, 800, 600);
}

int Gdestokp::createMainMenu(Grect* mainMenu) {
    windows = mainMenu;
}

int Gdestokp::activate() {
    int command = 0, pidServidor, status, parameter2;
    sendCommand(cmsCreateConnection, pidServer, 0, 0); //Solicitud de Conexion con el servidor
    getCommand(&command, &pidServidor, &status, &parameter2);
    if (status == 0) { //Se acepto la conexion
        writeBufferIPC(pidClient, (char*) windows, 0, sizeof (Grect));
        sendCommand(cmsCreateWindows, pidServer, 1, 0);
        getCommand(&command, &pidServidor, &status, &parameter2);
        if (status == 0) { //Se creo la ventana
            while (1) {
                getCommand(&command, &pidServidor, &status, &parameter2);
                executeCommnd(command, pidServidor, status, parameter2);
            }
        }
    }
    return 0;
}

int Gdestokp::executeCommnd(int command, int pidServer, int parameter1, int parameter2) {
    switch (command) {
        case cmsPaintWindows:
        {
            draw();
            desktop->reasignPositionsChilds();
            return 0;
        }
        case cmsCreateWindows:
        {
            if (parameter1 != pidClient) {
                if (mainBar->activeButton) {
                    mainBar->activeButton->color = 0xc6c6c6;
                    mainBar->activeButton->refresh();
                }
                GmainBarButton *newButton = new GmainBarButton(0, 1, 40, mainBar->height - 4);
                newButton->color = 0xFF00FF00;
                newButton->initialized();
                mainBar->activeButton = newButton;
                mainBar->addToolButton(newButton);
                newButton->tag = parameter1;
                newButton->wsState = wsNormal;
                mainBar->refresh();
            }
            return 0;
        }
        case cmsActiveWindows:
        {
            for (int i = 0; i < mainBar->toolButtons.count(); i++) {
                if (mainBar->toolButtons[i]->tag == parameter1) {
                    if (mainBar->activeButton) {
                        mainBar->activeButton->color = 0xc6c6c6;
                        mainBar->activeButton->refresh();
                    }
                    mainBar->toolButtons[i]->color = 0xFF00FF00;
                    mainBar->toolButtons[i]->refresh();
                    mainBar->activeButton = mainBar->toolButtons[i];
                    return 0;
                }
            }
            return 0;
        }
        case cmsCloseWindows:
        {
            if (controlMouseMove == (Gcontrol*) mainBar->activeButton) controlMouseMove = 0;
            if (mainBar->activeButton->tag == parameter1) mainBar->activeButton = 0;
            for (int i = 0; i < mainBar->toolButtons.count(); i++) {
                if (mainBar->toolButtons[i]->tag == parameter1) {
                    mainBar->deleteToolButton(mainBar->toolButtons[i]);
                    mainBar->refresh();
                    return 0;
                }
            }
            return 0;
        }
        case cmsMinimizedWindows:
        {
            mainBar->activeButton->wsState = wsMaximized;
            return 0;
        }
        case cmeClickDown:
        {
            Gcontrol *selectedControl = desktop->getControl(parameter1, parameter2);
            if (controlFocused && controlFocused != selectedControl) {
                controlFocused->focus = false;
                this->onFocused(controlFocused);
            }
            controlFocused = selectedControl;
            selectedControl->focus = true;
            this->onClickDown(selectedControl, parameter1, parameter2);
            return 0;
        }
        case cmeClickUp:
        {
            Gcontrol *selectedControl = desktop->getControl(parameter1, parameter2);
            this->onClickUp(selectedControl, parameter1, parameter2);
            return 0;
        }
        case cmeDoubleClick:
        {
            /* Gcontrol *selectedControl = getControl(windows, parameter1, parameter2);
             onDoubleClick(selectedControl, parameter1, parameter2);*/
            return 0;
        }
        case cmeKeyDown:
        {
            /*   int key = parameter1;
               if (controlFocused)
                   onKeyDown(controlFocused, key);*/
            return 0;
        }
        case cmeMauseMove:
        {
            if (controlMouseMove)
                leaveMouseMove(controlMouseMove);
            Gcontrol *selectedControl = desktop->getControl(parameter1, parameter2);
            controlMouseMove = selectedControl;
            this->onMouseMove(selectedControl, parameter1, parameter2);
            return 0;
        }
        case cmsRepaintZone:
        {
            Grect *newZone;
            readBufferIPC(pidClient, (char*) newZone, 0, sizeof (Grect));
            desktop->repaintZone(*newZone);
            return 0;
        }
        default: return 0;
    }
}

Gdestokp::~Gdestokp() {
}

void Gdestokp::draw() {
    desktop = new Gscreen();
    desktop->setResolution(800, 600);
    desktop->setColorBackGround(0x1dbce4);
    desktop->refresh();
    mainBar = new GmainBar(0, 550, 800, 20);
    mainBar->initialized();
    mainBar->refresh();
    desktop->insertControl(mainBar);
}

void Gdestokp::leaveMouseMove(Gcontrol* sender) {
    if (sender->classNameIs("GmainBarButton")) {
        ((GmainBarButton*) sender)->refresh();
    }
}

void Gdestokp::onMouseMove(Gcontrol* sender, int x, int y) {
    if (sender->classNameIs("GmainBarButton")) {
        ((GmainBarButton*) sender)->mouseMove(sender, x, y);
    }
}

void Gdestokp::onClickDown(Gcontrol* sender, int x, int y) {

}

void Gdestokp::onFocused(Gcontrol* sender) {

}

void Gdestokp::onClickUp(Gcontrol* sender, int x, int y) {
    if (sender->classNameIs("GmainBarButton")) {
        if (((GmainBarButton*) sender)->wsState == wsNormal && ((GmainBarButton*) sender) == mainBar->activeButton) {
            sendCommand(cmsMinimizedWindows, pidServer, ((GmainBarButton*) sender)->tag, 0);
            ((GmainBarButton*) sender)->wsState = wsMaximized;
        } else {
            sendCommand(cmsRestoreWindows, pidServer, ((GmainBarButton*) sender)->tag, 0);
            ((GmainBarButton*) sender)->wsState = wsNormal;
        }
    }
}