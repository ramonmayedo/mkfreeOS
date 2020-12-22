/*Copyright (C) 2019  Ramï¿½n Mayedo Morales (ramonmayedo@gmail.com)

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
#include "../../client/Gclient.h"
#include "../../components/Gwindows.h"
#include "../../components/Gbutton.h"
#include "../../components/GlistView.h"
#include "../../components/GlineEdit.h"
void Form(Gwindow *windows);

Gclient *application;
Gbutton *buton1, *button2;

int kmain(int argc, char *argv[]) {
    application = new Gclient();
    Gwindow * windows = new Gwindow(50, 50, 400, 400, 0);
    GlineEdit *edit = new GlineEdit(5,5,200,20, windows);
    edit->setText("Edit 1");
    edit->setAlign(caTop);
    buton1 =  new Gbutton(260,40,100,40, windows);
    buton1->caption = "Boton";
    GlistView *listView = new GlistView(40,60,300,200,windows);
    Gcolumn *name = new Gcolumn(200);
    name->caption = "Name";
    listView->addColumn(name);
    name = new Gcolumn(200);
    name->caption = "Type";
    listView->addColumn(name);   
   name = new Gcolumn(100);
    name->caption = "Type";
    listView->addColumn(name);
    windows->caption = "Notepad";
    windows->iconPath = "a/not.png";
    if (application->createWindows(windows)) {
        windows->paint();
        windows->repaint();
        application->show(windows);
    }
    application->activate();
    return 0;
}

void Form(Gwindow *windows) {

}