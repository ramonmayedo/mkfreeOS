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

#ifndef GWINDOWS_H
#define GWINDOWS_H
#include "../basics/grect.h"
#include "../basics/canvasPainter.h"
#include "Gcomponent.h"

class Gwindow : public Gcomponent {
public:
    Gwindow(int x, int y, int width, int height, Gcomponent *aparent);
    Grect *getWindowsRect();
    void resize(int width, int height);
    void setdoubleBuffer(bool active);
    void setBackCanvas(int *buffer);
    void onCreate();
    void flush();
    void flushRect(Grect &rect);
    void draw();
    int keyShmem;
    int wid;
    GcanvasPainter *backCanvas;
    GcanvasPainter *frontCanvas;
    GgraphicComponent *controlFocused;
    string caption;
    string iconPath;
    ~Gwindow();
private:
    bool doubleBuffer;    

};

#endif

