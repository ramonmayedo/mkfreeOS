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
#include "../../systemWindows/Gclient.h"
#include "../../components/Gwindows.h"
#include "../../components/Gpanel.h"
#include "../../components/GtextEdit.h"
#include "../../components/GstatusBar.h"
#include "../../../library/includes/dirent.h"
#include "../../components/GtoolBar.h"

void Form(Gwindows *windows);
void windowsOnCreate(Gcontrol *sender);

int kmain(int argc, char *argv[]) {
    Gclient *application = new Gclient();
    Gwindows * windows = new Gwindows(440, 50, 500, 500);
    windows->OnCreate = windowsOnCreate;
    application->createWindows(windows);
    application->OnDraw = Form;
    application->activate();
    return 0;
}
GstatusBar *statusBar;
GtoolBar *mainToolBar;
GtextEdit *textEdit;

void textEditOnKeyPress(Gcontrol *sender, int key) {
    statusBar->panels[0]->text = ((GtextEdit*) sender)->pcursor.y() + 1;
    statusBar->panels[0]->text += ":";
    statusBar->panels[0]->text += ((GtextEdit*) sender)->pcursor.x() + 1;
    statusBar->refresh();
}

void textEditOnClickDown(Gcontrol *sender, int x, int y) {
    int key;
    textEditOnKeyPress(sender, key);
}

void windowsOnCreate(Gcontrol *sender) {
    GtoolButton *toolButton = new GtoolButton(0, 1, 40, mainToolBar->height - 4);
    toolButton->initialized();
    mainToolBar->addToolButton(toolButton);
    mainToolBar->refresh();
    GstatusPanel *panel1 = new GstatusPanel(200);
    GstatusPanel *panel2 = new GstatusPanel(220);
    statusBar->addPanel(panel1);
    statusBar->addPanel(panel2);
    statusBar->refresh();
}

void Form(Gwindows *windows) {
    windows->caption = "Text Edit";
    mainToolBar = new GtoolBar(0,25,windows->width-2,25);
    mainToolBar->initialized();
    textEdit = new GtextEdit(5, 50, windows->width - 10, windows->height - 75);
    textEdit->initialized();
    textEdit->onKeyPress = textEditOnKeyPress;
    textEdit->onClickDown = textEditOnClickDown;
    statusBar = new GstatusBar(0,windows->height - 25,windows->width-2,25);
    statusBar->initialized();
    windows->insertControl(statusBar);
    windows->insertControl(textEdit);
    windows->insertControl(mainToolBar);
}