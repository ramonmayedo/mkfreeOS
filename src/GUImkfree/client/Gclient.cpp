#include "Gclient.h"
#include "../../library/includes/unistd.h"
#include "../../library/includes/stdio.h"

Gclient::Gclient() {
    int command, pidServidor, parameter1, parameter2;
    do {
        getCommand(&command, &pidServidor, &parameter1, &parameter2);
    } while (command != csSetPIDServer);
    pidServer = parameter1;
    pidClient = getPid();
    sendCommand(csCreateConnection, pidServer, 0, 0); //Solicitud de Conexion con el servidor
    getCommand(&command, &pidServidor, &parameter2, &parameter2); //Espera de aceptacion por el servidor
}

int Gclient::createWindows(Gwindow* windows) {
    int command, pidServidor, par1, par2;
    Grect *rect = windows->getWindowsRect(); //Se crea una ventana
    writeBufferIPC(pidClient, (char*) rect, 0, sizeof (Grect)); //Posicion y tamaño de la ventana
    sendCommand(csCreateWindows, pidServer, 1, 1); //Se envia al server
    getCommand(&command, &pidServidor, &par1, &par2); //Etado y identificador de ventana del server
    if (par1 != 0) return 0; //Se creo la ventana en el servidor
    windows->wid = par2; //Identificador del servidor para esta ventana, unico para cada ventana
    getCommand(&command, &pidServidor, &par1, &par2); //En espera de buffer compartido
    if (command != csSetSharedMemory) return 0;
    int shared, size;
    setshmem(par2, &shared, &size);
    windows->setBackCanvas((int*) par1);
    writeBufferIPC(pidClient, windows->caption.c_str(), 0, windows->caption.Lenght());
    sendCommand(csSetCaption, pidServer, windows->wid, windows->caption.Lenght());
    getCommand(&command, &pidServidor, &par1, &par2);
    writeBufferIPC(pidClient, windows->iconPath.c_str(), 0, windows->iconPath.Lenght());
    sendCommand(csSetIconPath, pidServer, windows->wid, windows->iconPath.Lenght());
    getCommand(&command, &pidServidor, &par1, &par2);
    this->windows = windows;
    windows->onCreate();
    return windows->wid;
}

void Gclient::resizeWindow(Gwindow* windows) {
    int command, pidServidor, par1, par2;
    getCommand(&command, &pidServidor, &par1, &par2);
    windows->backCanvas->canvas->resizeBitmap(par1, par2);
    windows->resize(par1, par2);
    sendCommand(csResizeWindow, pidServer, windows->wid, 1); //Se envia al server
}

int Gclient::show(Gwindow *windows) {
    sendCommand(csShowWindows, pidServer, windows->wid, 1); //Se envia al server
}

int Gclient::activate() {
    int command = 0, pidServidor, status, parameter2;
    do {
        getCommand(&command, &pidServidor, &status, &parameter2);
        executeCommnd(command, pidServidor, status, parameter2);
    } while (1);
    return 0;
}

int Gclient::executeCommnd(int command, int pidServer, int parameter1, int parameter2) {
    switch (command) {
        case csPaintWindows:
        {
            return 0;
        }
        case csActiveWindows:
        {
            return 0;
        }
        case csCloseWindows:
        {
            return 0;
        }
        case esClickDown:
        {
           GgraphicComponent *selectedControl = windows->getComponent(windows, parameter1, parameter2);
            if (windows->controlFocused != selectedControl) {
                windows->controlFocused->focus = false;
                selectedControl->focus = true;
                if (windows->controlFocused->onFocused)
                    windows->controlFocused->onFocused(windows->controlFocused);
                if (selectedControl->onFocused)
                    selectedControl->onFocused(selectedControl);
                windows->controlFocused = selectedControl;
            }
            if (selectedControl->onClickDown)
                selectedControl->onClickDown(selectedControl, parameter1, parameter2);
            return 0;
        }
        case esClickUp:
        {
            if (windows->controlFocused->onClickUp)
                windows->controlFocused->onClickUp(windows->controlFocused, parameter1, parameter2);
            return 0;
        }
        case esDoubleClick:
        {
            return 0;
        }
        case esKeyDown:
        {
            int key = parameter1;
           /* if (windows->controlFocused->onKeyDown)
                windows->controlFocused->onKeyDown(windows->controlFocused, key);*/
            return 0;
        }
        case esMauseMove:
        {
            GgraphicComponent *selectedControl = windows->getComponent(windows, parameter1, parameter2);
            // if (windows->controlMouseMove)
            // if (windows->controlMouseMove != selectedControl)
            //    leaveMouseMove(windows->controlMouseMove);
            // windows->controlMouseMove = selectedControl;
            if (selectedControl->onMouseMove)
                selectedControl->onMouseMove(selectedControl, parameter1, parameter2);
            return 0;
        }
        case csRepaintZone:
        {
            return 0;
        }
        case csResizeWindow:
        {
            windows->backCanvas->canvas->setGraphicsBitmap((int*) parameter2);
            resizeWindow(windows);
            return 0;
        }
        default: return 0;
    }
}

int Gclient::applicationCommand(int command) {
    return 0;
}

int Gclient::execApp(char* fileName, char* argv[]) {

}

void Gclient::refreshRect(Gwindow *window, Grect& rect) {
    writeBufferIPC(pidClient, (char*) &rect, 0, sizeof (Grect)); //Posicion y tamaño de la ventana
    sendCommand(csPaintRect, pidServer, 1, window->wid);         //Se envia al server
}