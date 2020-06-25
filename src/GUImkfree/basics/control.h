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

#ifndef CONTROL_H
#define CONTROL_H
#include "frame.h"
#include "list.h"
#include "object.h"
#include "cstring.h"

enum eborederStyle {
    bsNone, bsSingle
};

class Gcontrol : public Gobject {
    typedef void (eventOnClick) (Gcontrol*, int x, int y);
    typedef void (eventOnKeyPress) (Gcontrol*, int key);
    typedef void (eventOnFocused)(Gcontrol* sender);
public:
    Gcontrol();
    Gcontrol(int x, int y, int width, int height);
    void insertControl(Gcontrol *control);
    void removeControl(Gcontrol *control);
    void alignToPaint(Gcontrol *control, GAlign align);
    void repaintAllControlChilds();
    void repaintZone(Grect rect);
    Gcontrol *getControl(int x, int y);
    void *reasignPositionsChilds();
    int controlCount();
    ~Gcontrol();
    GAlign align;
    int left, top, width, height;
    int zindex;
    Gcontrol *parent;
    Gframe *graphics;
    Clist *controls;
    string name;  
    bool focus;
    eborederStyle border;
    eventOnClick *onClickDown;
    eventOnClick *onClickUp;
    eventOnKeyPress *onKeyPress;
    eventOnFocused *onFocused;
    eventOnClick *onMouseMove;
    eventOnClick *onDoubleClick;
private:
    
protected:
    void initialize();
    Gcontrol *getAllPosition(Gcontrol *control, int dx, int dy);
    Gcontrol *getControl(Gcontrol *control, int x, int y);
};

#endif

