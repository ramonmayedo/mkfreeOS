#ifndef CURSOR_H
#define CURSOR_H
#include "../basics/grect.h"
#include "cursor.h"
#include "../basics/gbitmap.h"
#include "../../library/includes/conio.h"

enum eCursorType {
    ctArrow=0, ctResizeDiagonalLeft, ctResizeVertical, ctResizeDiagonalRight,
    ctResizeHorizontal
};

class Gcursor {
public:
    Gcursor(char *fileName);
    Gcursor(char *fileName,int dx, int dy);
    SvideoArea areaCursor;
    GgraphicsBitmap *cursorBitmap;
    int corX, corY;
private:
    
};

#endif

