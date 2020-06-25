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

#include "columns.h"

GlistItem::GlistItem() {

}

GlistItem::~GlistItem() {
    subItem.clear();
}

GlistItems::GlistItems() {
    listItems = new Clist();
}

void GlistItems::add(GlistItem* listItem) {
    listItems->add(listItem);
}

GlistItem* GlistItems::get(int index) {
    return (GlistItem*)listItems->getItem(index);
}

GlistItem* GlistItems::operator[](int index) {
    return (GlistItem*) listItems->getItem(index);
}

int GlistItems::count() {
    return listItems->count();
}

void GlistItems::clear() {
    for (int i = 0; i < listItems->count(); i++)
        delete(operator[](i));
    listItems->clear();
}

GlistItems::~GlistItems() {

}

Gcolumn::Gcolumn() {

}

Gcolumn::Gcolumn(int cwidth) {
    width = cwidth;
}

Gcolumn::~Gcolumn() {

}

Gcolumns::Gcolumns() {
    columns = new Clist();
}

void Gcolumns::add(Gcolumn* newColumn) {
   columns->add(newColumn);
}

Gcolumn* Gcolumns::operator[](int index) {
    return (Gcolumn*)columns->getItem(index);
}

Gcolumn* Gcolumns::get(int index) {
    return (Gcolumn*)columns->getItem(index);
}

int Gcolumns::count() {
    return columns->count();
}


Gcolumns::~Gcolumns() {
}

GstatusPanel::GstatusPanel(){
    
}

GstatusPanel::GstatusPanel(int width) {
    this->width = width;
}

Gpanels::Gpanels() {
    panels = new Clist();
}

void Gpanels::add(GstatusPanel* newPanel) {
    panels->add(newPanel);
}

GstatusPanel* Gpanels::get(int index) {
    return (GstatusPanel*) panels->getItem(index);
}

GstatusPanel* Gpanels::operator[](int index) {
    return (GstatusPanel*) panels->getItem(index);
}

int Gpanels::count() {
    return panels->count();
}

Gpanels::~Gpanels() {

}