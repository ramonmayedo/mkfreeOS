#ifndef canvasPainter
#define canvasPainter
#include "grect.h"
#include "gbitmap.h"
#include "font.h"
#include "cstring.h"

class GcanvasPainter {
public:
    GcanvasPainter(GgraphicsBitmap *acanvas);
    int drawRect(Grect rect, Gcolor color);
    int drawRectBorder(Grect rect, Gcolor colorRect, Gcolor colorBorder);
    int drawRectGradient(Grect rect, Gcolor color, Gcolor color1);
    int borderColor(int width, Grect rect, Gcolor color);
    int drawText(Grect rect, int margin, Gfont *font, string text, Gcolor color, int textAlignment);
    int drawGlyp(Grect rect, int margin, Gfont *font, char car, Gcolor color);
    int drawBitmap(GgraphicsBitmap *bitmap, Grect rect, int x, int y);
    int drawBitmapTransparent(GgraphicsBitmap *bitmap, Grect rect, Gcolor color);
    int changeBackGroundColorBitmap(GgraphicsBitmap *bitmap, Gcolor oldColor, Gcolor newColor);
    int drawBitmapAlpha(GgraphicsBitmap *bitmap, Grect rect, int x, int y);
    int drawBitmap(GgraphicsBitmap *bitmap, Grect rect);
    int backgroundColor(Gcolor color);
    int copyBitmap(GgraphicsBitmap *bitmap);
    void iputc(int x, int y, Gcolor &color, Gfont &font, const char c);
    void iputc(int x, int y, Gcolor &fontColor, Gcolor &backGroundColor, Gfont &font, const char c);
    void setCanvasRect(Grect &rect);
    GgraphicsBitmap *canvas;
    Grect  cutRect;
    Gpoint pointRect;
    ~GcanvasPainter();
private:

};
#endif
