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

#include "../../../library/includes/cprogram.h"
#include "../../../library/includes/stdlib.h"
#include "../../systemWindows/Gclient.h"
#include "../../components/Gwindows.h"
#include "../../components/Gpanel.h"
#include "../../components/GlistView.h"
#include "../../components/GlineEdit.h"
#include "../../components/GstatusBar.h"
#include "../../../library/includes/dirent.h"
#include "../../components/GtoolBar.h"

void Form(Gwindows *windows);
void windowsOnCreate(Gcontrol *sender);

Gclient *application;

int kmain(int argc, char *argv[]) {
    application = new Gclient();
    Gwindows * windows = new Gwindows(30, 30, 500, 500);
    windows->OnCreate = windowsOnCreate;
    application->createWindows(windows);
    application->OnDraw = Form;
    application->activate();
    return 0;
}
GlineEdit *pathText;
GlistView *listExplorer;
GstatusBar *statusBar;
GtoolBar *mainToolBar;
GtoolButton *execToolButton, *shutDownToolButton, *resetToolButton;


void pathTextOnKeyPress(Gcontrol *sender, int key) {
     int countFiles=0,countFolder=0,fsize=0;
    if (key == 0x0A) {
        DIR *dir = opendir(pathText->text.c_str());
        string *nuevo, *typeFile, *size, *typeFolder;
        typeFile = new string("Archivo");
        typeFolder = new string("Carpeta");
        dirent *archivo;
        if (dir) {
            listExplorer->items.clear();
            while ((archivo = readdir(dir))) {
                nuevo = new string(archivo->name);
                size = new string((int) archivo->fileSize);
                GlistItem *newItem = new GlistItem();
                newItem->caption = archivo->name;
                if (archivo->typeFile == ATTRK_ARCHIVE) {
                    countFiles++;
                    fsize += archivo->fileSize;
                    newItem->subItem.add(typeFile);
                } else {
                    newItem->subItem.add(typeFolder);
                    countFolder++;
                    *size = "";
                }               
                newItem->subItem.add(size);
                listExplorer->items.add(newItem);
            }
            closedir(dir);
            listExplorer->load();
            listExplorer->refresh();
            statusBar->panels[0]->text = "Size All(";
            statusBar->panels[0]->text += fsize;
            statusBar->panels[0]->text += ")";
            statusBar->panels[1]->text = "Files(";
            statusBar->panels[1]->text += countFiles;
            statusBar->panels[1]->text += ") Folders(";
            statusBar->panels[1]->text += countFolder;
            statusBar->panels[1]->text += ")";
            statusBar->refresh();
            //dirPath = pathText->text;
        }else{
//            pathText->text = dirPath;
  //          pathText->refresh();
        }
    }
}

void listExplorerEdited(Gcontrol *sender, GlistItem *item, string &newString, bool &accept) {
    string name = pathText->text;
    string newName = name;
    if (pathText->text[pathText->text.Lenght() - 1] != '/') {
        newName += "/";
        name += "/";
    }
    newName += newString;
    name += item->caption;
    if (!rename(name.c_str(), newName.c_str())) return;
    accept = false;
}

void listExplorerOnKeyPress(Gcontrol *sender, int key) {
    if (key == 0x0A && listExplorer->itemIndex != -1) {
        string newPath = pathText->text;
        if (pathText->text[pathText->text.Lenght() - 1] != '/')
            newPath += "/";
        newPath += listExplorer->items[listExplorer->itemIndex]->caption;
        DIR *dir = opendir(newPath.c_str());
        if (dir) {
            closedir(dir);
            pathText->text = newPath;
            pathText->refresh();
            pathTextOnKeyPress(sender, key);
        }
    }
}

void listExplorerOnDoubleClick(Gcontrol *sender, int x, int y) {
    if (listExplorer->itemIndex != -1) {
        string newPath = pathText->text;
        if (newPath[newPath.Lenght() - 1] != '/')
            newPath += "/";
        newPath += listExplorer->items[listExplorer->itemIndex]->caption;
        DIR *dir = opendir(newPath.c_str());
        if (dir) {
            closedir(dir);
            pathText->text = newPath;
            pathText->onKeyPress(sender, 0xA);
            pathText->refresh();
        }
    }
}

void listExplorerOnClickDown(Gcontrol *sender, int x , int y) {
    //statusBar->panels[0]->text = *listExplorer->columns[0]->items[listExplorer->itemIndex];
   // statusBar->refresh();
}

void execToolButtonOnCLickUp(Gcontrol *sender, int x, int y) {
    if (listExplorer->itemIndex != -1) {
        string name = pathText->text;
        if (pathText->text[pathText->text.Lenght() - 1] != '/') name += "/";
        name += listExplorer->items[listExplorer->itemIndex]->caption;
        char **argv = 0;
        application->execApp(name.c_str(), argv);
    }
}
void shutDownToolButtonOnCLickUp(Gcontrol *sender, int x, int y){
    reset();
}

void windowsOnCreate(Gcontrol *sender) {
    execToolButton = new GtoolButton(0, 1, 40, mainToolBar->height - 4);
    execToolButton->initialized();
    execToolButton->onClickUp = execToolButtonOnCLickUp;
    
    shutDownToolButton = new GtoolButton(0, 1, 40, mainToolBar->height - 4);
    shutDownToolButton->initialized();
    shutDownToolButton->onClickUp = shutDownToolButtonOnCLickUp;
    
    mainToolBar->addToolButton(execToolButton);
    mainToolBar->addToolButton(shutDownToolButton);
    mainToolBar->refresh();
    
    GstatusPanel *panel1 = new GstatusPanel(200);
    GstatusPanel *panel2 = new GstatusPanel(220);
    statusBar->addPanel(panel1);
    statusBar->addPanel(panel2);
    statusBar->refresh();
    Gcolumn *col = new Gcolumn(180);
    col->caption = "Nombre";
    listExplorer->addColumn(col);
    Gcolumn *col1 = new Gcolumn(150);
    col1->caption = "Tipo";
    listExplorer->addColumn(col1);
    col1 = new Gcolumn(120);
    col1->caption = "Size";
    listExplorer->addColumn(col1);
    pathText->text = "a/";
    pathText->refresh();
    //dirPath = "a/";
    pathTextOnKeyPress(sender, 0xA);  
}

void Form(Gwindows *windows) {
    windows->caption = "File Explorer";
    mainToolBar = new GtoolBar(0,25,windows->width,25);
    mainToolBar->initialized();
    pathText = new GlineEdit(5, 55, windows->width - 10, 22);
    pathText->initialize();
    pathText->onKeyPress = pathTextOnKeyPress;
    listExplorer = new GlistView(5, 85, windows->width - 10, windows->height - 115);
    listExplorer->initialize();
    listExplorer->onKeyPress = listExplorerOnKeyPress;
    listExplorer->onClickDown = listExplorerOnClickDown;
    listExplorer->OnChangeItem = listExplorerEdited;
    listExplorer->onDoubleClick = listExplorerOnDoubleClick;
    statusBar = new GstatusBar(0,windows->height - 25,windows->width-2,25);
    statusBar->initialized();
    windows->insertControl(pathText);
    windows->insertControl(listExplorer);
    windows->insertControl(statusBar);
    windows->insertControl(mainToolBar);
}