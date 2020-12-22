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
#include "../../client/Gclient.h"
#include "../../components/Gwindows.h"
#include "../../components/Gbutton.h"
#include "../../components/GlistView.h"

void Form(Gwindow *windows);

Gclient *application;

int kmain(int argc, char *argv[]) {
    application = new Gclient();
    Gwindow * windows = new Gwindow(50, 50, 500, 500, 0);
    windows->caption = "File Explorer";
    windows->iconPath = "a/cons.png";
    Gbutton *button1 = new Gbutton(400,20, 200, 60, windows);
    button1->caption ="Boton Ok";
    Gbutton *button = new Gbutton(5,5, 20, 20, button1);
    button->caption="A";
    GlistView *listView = new GlistView(40,80,300,200,windows);
    Gcolumn *name = new Gcolumn(200);
    name->caption = "Name";
    listView->addColumn(name);
    name = new Gcolumn(200);
    name->caption = "Type";
    listView->addColumn(name);   
   name = new Gcolumn(100);
    name->caption = "Type";
    listView->addColumn(name);

    button1 = new Gbutton(20,80, 100, 60, windows);
    button1->caption = "Cancel";
    
    if (application->createWindows(windows)) {
        windows->resizeComponent();
        application->show(windows);
    }
    application->activate();
    return 0;
}

void Form(Gwindow *windows) {

}