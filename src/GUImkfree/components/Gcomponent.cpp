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
#include "Gcomponent.h"

Gcomponent::Gcomponent(int x, int y, int width, int height, GgraphicComponent *parent) : GgraphicComponent(x, y, width, height, parent) {

}

void Gcomponent::setLeft(int x) {
    realRect.setx(x);
    //point.setx(x);
    //rebuildComponents();
}

Gcomponent::~Gcomponent() {
}

