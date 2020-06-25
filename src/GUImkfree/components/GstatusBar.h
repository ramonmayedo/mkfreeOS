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

#ifndef GSTATUSBAR_H
#define GSTATUSBAR_H
#include "../basics/control.h"
#include "../basics/columns.h"
#include "../basics/kgraphics.h"
class GstatusBar : public Gcontrol {
public:
    GstatusBar();
    GstatusBar(int x, int y, int width, int height);
    void initialized();
    void refresh();
    void addPanel(GstatusPanel *panel);
    void repaint();
    ~GstatusBar();
    Gcolor color;
    Gpanels panels;
private: 
    Gfont *data;
    int charWidth;
};

#endif

