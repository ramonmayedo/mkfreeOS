#include "gbitmap.h"
#include "canvasPainter.h"

GgraphicsBitmap::GgraphicsBitmap(int width, int height) : Grect(0,0, width, height) {
    glinearBuffer = new int[(width + 1)*(height + 1)];
    isExternGrpahics = false;
}

GgraphicsBitmap::GgraphicsBitmap(Gfont *font, Gcolor bkColor, Gcolor fnColor, const char car) : Grect(0, 0, font->width, font->height) {
    glinearBuffer = new int[(font->width + 1)*(font->height + 1)];
    GcanvasPainter painter(this);
    painter.backgroundColor(bkColor);
    painter.drawGlyp(Grect(0, 0, 0, 0), 0, font, car, fnColor);
}

GgraphicsBitmap::GgraphicsBitmap(Grect &rect, Gfont *font, Gcolor bkColor, Gcolor fnColor, const char car) : Grect(rect) {
    glinearBuffer = new int[(rect.width() + 1)*(rect.height() + 1)];
    GcanvasPainter painter(this);
    painter.backgroundColor(bkColor);
    painter.drawGlyp(Grect(0, 0, 0, 0), 0, font, car, fnColor);
}

GgraphicsBitmap::GgraphicsBitmap(int width, int height, int *externLinearBuffer) : Grect(0, 0, width, height) {
    glinearBuffer = externLinearBuffer;
    isExternGrpahics = true;
}

void GgraphicsBitmap::setGraphicsBitmap(int *externGraphicsBitmap){
     glinearBuffer =  externGraphicsBitmap;
     isExternGrpahics = true;
}

void GgraphicsBitmap::resizeBitmap(int width,int height){
    setwidth(width);
    setheight(height);
    if (!isExternGrpahics){
       delete(glinearBuffer);
       glinearBuffer = new int[(width + 1)*(height + 1)];
    }
}

int *GgraphicsBitmap::getLine(int y){
    return &glinearBuffer[y*width()];
}

int* GgraphicsBitmap::getGraphicsBitmap() {
    return glinearBuffer;
}

GgraphicsBitmap::~GgraphicsBitmap() {
    if (!isExternGrpahics)
       delete(glinearBuffer);
}