#include "cursor.h"
#include "../../library/includes/stdio.h"
#include "../basics/png.h"

Gcursor::Gcursor(char *fileName) {
    cursorBitmap = Gpng().loadPNG(fileName);
    areaCursor.area = cursorBitmap->getGraphicsBitmap();
    areaCursor.height = areaCursor.trueHeight = cursorBitmap->height();
    areaCursor.width = areaCursor.trueWidth = cursorBitmap->width();
    areaCursor.left = areaCursor.trueLeft = 0;
    areaCursor.top = areaCursor.trueTop = 0;
    corX = 0;
    corY = 0;
}

Gcursor::Gcursor(char* fileName, int dx, int dy) {
    cursorBitmap = Gpng().loadPNG(fileName);
    areaCursor.area = cursorBitmap->getGraphicsBitmap();
    areaCursor.height = areaCursor.trueHeight = cursorBitmap->height();
    areaCursor.width = areaCursor.trueWidth = cursorBitmap->width();
    areaCursor.left = areaCursor.trueLeft = 0;
    areaCursor.top = areaCursor.trueTop = 0;
    if (dy) corY = areaCursor.height / dy;
    else corY = 0;
    if (dx) corX = areaCursor.width / dx;
    else corX = 0;
}
