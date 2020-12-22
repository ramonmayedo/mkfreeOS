#include "canvasPainter.h"
#include "templates.h"
#include "png.h"

GcanvasPainter::GcanvasPainter(GgraphicsBitmap *acanvas) {
    canvas = acanvas;
    cutRect = *canvas;
    pointRect = *canvas;
}

int GcanvasPainter::drawRect(Grect rect, Gcolor color) {
    rect.move(pointRect);
    rect &= cutRect;
    if (rect.isEmpty()) return 0;
    int *linearBuffer = canvas->getLine(rect.top()) + rect.left();
    int newColor = color.valColor();
    int canvasWidth = canvas->width();
    int rectWidth = rect.width();
    for (int i = 0; i < rect.height(); i++) {
        fastFill32((u32*) linearBuffer, (u32) newColor, (u32) rectWidth);
        linearBuffer += canvasWidth;
    }
    return 0;
}

int GcanvasPainter::drawRectBorder(Grect rect, Gcolor colorRect, Gcolor colorBorder) {
    Grect newRect = rect;
    newRect.move(pointRect);
    newRect &= cutRect;
    if (newRect.isEmpty()) return 0;
    int *linearBuffer = canvas->getLine(rect.top()) + rect.left();
    int backColor = colorRect.valColor();
    int colorBor = colorBorder.valColor();
    int color;
    for (int i = 0; i < newRect.height(); i++) {
        color = ((i == 0 || i == newRect.height() - 1) ? colorBor : backColor);
        linearBuffer[0] = colorBor;
        linearBuffer[newRect.width() - 1] = colorBor;
        fastFill32((u32*) linearBuffer + 1, (u32) color, (u32) newRect.width() - 2);
        linearBuffer += canvas->width();
    }
    return 0;
}

int GcanvasPainter::drawRectGradient(Grect rect, Gcolor color, Gcolor color1) {
    Grect newRect = rect;
    newRect.move(pointRect);
    newRect &= cutRect;
    if (newRect.isEmpty()) return 0;
    int g, b, r;
    int *linearBuffer = canvas->getLine(rect.top()) + rect.left();
    int newColor;
    float c = color.red();
    float d = color.green();
    float e = color.blue();
    float r1 = (float) (color1.red() - color.red()) / (float) newRect.height();
    float g1 = (float) (color1.green() - color.green()) / (float) newRect.height();
    float b1 = (float) (color1.blue() - color.blue()) / (float) newRect.height();
    int canvasWidth = canvas->width();
    for (int i = 0; i < newRect.height(); i++) {
        r = (u8) (c + r1 * i);
        g = (u8) (d + g1 * i);
        b = (u8) (e + b1 * i);
        newColor = Gcolor(r, g, b).valColor();
        fastFill32((u32*) linearBuffer, (u32) newColor, (u32) newRect.width());
        linearBuffer += canvasWidth;
    }
    return 0;
}

int GcanvasPainter::borderColor(int width,Grect rect, Gcolor color) {
    drawRect(Grect(0, 0, rect.width(), width), color);
    drawRect(Grect(0, 0, width, rect.height()), color);
    drawRect(Grect(0, rect.height() - width,rect.width(), width), color);
    drawRect(Grect(rect.width() - width, 0, width, rect.height()), color);
    return 0;
}

int GcanvasPainter::drawBitmap(GgraphicsBitmap *bitmap, Grect rect, int x, int y) {
    rect.move(pointRect);
    rect &= cutRect;
    if (rect.isEmpty()) return 0;
    int *buffer = bitmap->getLine(y) + x;
    int left = rect.left();
    int top = rect.top();
    if (rect.intersects(cutRect)) {
        if (left < 0) {
            buffer += (-left);
            left = 0;
        }
        if (top < 0) {
            buffer += (-top) * bitmap->width();
            top = 0;
        }
    }
    int *bufferDes = canvas->getLine(top) + left;
    int canvasWidth = canvas->width();
    int rectWidth = rect.width();
    for (int i = 0; i < rect.height(); i++) {
        fastCopy((u32*) bufferDes, (u32*) buffer, (u32) rectWidth);
        buffer += bitmap->width();
        bufferDes += canvasWidth;
    }
    return 1;
}

int GcanvasPainter::drawBitmapTransparent(GgraphicsBitmap *bitmap, Grect rect, Gcolor color) {
    if (rect.isEmpty())
        return 0;
    rect.move(cutRect);
    int *linearBuffer = canvas->getLine(rect.top()) + rect.left();
    int *linerBufferOrigen = bitmap->getLine(0);
    int valColor = color.valColor();
    int canvasWidth = canvas->width();
    int bitmapWidth = bitmap->width();
    for (int i = 0; i < rect.height(); i++) {
        for (int j = 0; j < rect.width(); j++)
            if (linerBufferOrigen[j] != valColor)
                linearBuffer[j] = linerBufferOrigen[j];
        linearBuffer += canvasWidth;
        linerBufferOrigen += bitmapWidth;
    }
}

int GcanvasPainter::changeBackGroundColorBitmap(GgraphicsBitmap* bitmap, Gcolor oldColor, Gcolor newColor) {
    int *linearBuffer = bitmap->getGraphicsBitmap();
    int valOldColor = oldColor.valColor();
    int valNewColor = newColor.valColor();
    for (int i = 0; i < bitmap->width() * bitmap->height(); i++) {
        if (linearBuffer[i] == valOldColor)
            linearBuffer[i] = valNewColor;
    }
}


int GcanvasPainter::drawBitmapAlpha(GgraphicsBitmap* bitmap, Grect rect, int x, int y) {
    if (rect.isEmpty())
        return 0;
    Scolor32 *linearBuffer = (Scolor32*) (canvas->getLine(rect.top()) + rect.left());
    Scolor32 *linerBufferOrigen = (Scolor32*) bitmap->getLine(0);
    for (int i = 0; i < rect.height(); i++) {
        for (int j = 0; j < rect.width(); j++)
            if (linerBufferOrigen[j].a != 0x0)
                linearBuffer[j] = linerBufferOrigen[j];
        linearBuffer += canvas->width();
        linerBufferOrigen += bitmap->width();
    }
}

int GcanvasPainter::drawBitmap(GgraphicsBitmap* bitmap, Grect rect) {
    drawBitmap(bitmap, rect, 0, 0);
}

int GcanvasPainter::drawText(Grect rect, int margin, Gfont *font, string text, Gcolor color, int textAlignment) {
    Grect newRect = rect;
    newRect.move(pointRect);

    if (newRect.isEmpty()) return 0;
    int maxWidth = (text.Lenght() - 1)*(font->width + font->spaceSeparator);
    Grect rectAlign = newRect;
    rectAlign.setheight(font->height);
    if (maxWidth < rectAlign.width()) rectAlign.setwidth(maxWidth);

    switch (textAlignment) {
        case talignCenter:
        {
            rectAlign.alignCenter(newRect);
            break;
        }
        case talignCenterTop:
        {
            rectAlign.alignCenterVertical(newRect);
            break;
        }
        case talignCenterBottom:
        {
            rectAlign.alignBottom(newRect);
            rectAlign.alignCenterHorizontal(newRect);
            break;
        }
        default: break;
    };
    newRect &= cutRect;
    
    bool wordWrap = false;
    char *ptr = text.c_str();
    int spaceSeparator = font->spaceSeparator;
    int cwidth = newRect.width() / (font->width + spaceSeparator);
    int cheight = newRect.height() / (font->height + spaceSeparator);
    int px = rectAlign.x() + margin;
    int py = rectAlign.y();
    int x = 0, y = 0;
    while (*ptr) {
        if (x < cwidth) {
            iputc(px, py, color, *font, *ptr);
            px += font->width + spaceSeparator;
        };
        x++;
        ptr++;
        if (x == cwidth && wordWrap == true) {
            y++;
            if (y == cheight) return 0;
            px = rectAlign.x() + spaceSeparator;
            x = 0;
            py += font->height + spaceSeparator;
        }
    }
}

int GcanvasPainter::drawGlyp(Grect rect, int margin, Gfont* font, char car, Gcolor color) {
    iputc(rect.x() + margin, rect.top(), color, *font, car);
}

void GcanvasPainter::iputc(int x, int y, Gcolor &color, Gfont &font, const char c) {
    int *linearBuffer = canvas->getLine(y) + x;
    int *fontMap = &font.linearBuffer[c * font.height];
    int newColor = color.valColor();
    char i, j, k;
    for (i = 0; i < font.height; i++) {
        for (j = 0, k = font.width; j < font.width; j++, k--)
            if (fontMap[i] & (1 << k))
                linearBuffer[j] = newColor;
        linearBuffer += canvas->width();
    }
}

void GcanvasPainter::iputc(int x, int y, Gcolor &fontColor, Gcolor &backGroundColor, Gfont& font, const char c) {
    int *linearBuffer = canvas->getLine(y) + x;
    int *fontMap = &font.linearBuffer[c * font.height];
    int fColor = fontColor.valColor();
    int bColor = backGroundColor.valColor();
    char i, j, k;
    for (i = 0; i < font.height; i++) {
        for (j = 0, k = font.width; j <= font.width; j++, k--) {
            if (fontMap[i] & (1 << k))
                linearBuffer[j] = fColor;
            else
                linearBuffer[j] = bColor;
        }
        linearBuffer += canvas->width();
    }
}

int GcanvasPainter::backgroundColor(Gcolor color) {
    int newColor = color.valColor();
    int *linearBuffer = canvas->getGraphicsBitmap();
    int size = canvas->width() * canvas->height();
    fastFill32((u32*) linearBuffer, (u32) newColor, (u32) size);
}

int GcanvasPainter::copyBitmap(GgraphicsBitmap* bitmap) {
    int *linearBuffer = canvas->getGraphicsBitmap();
    int *linearBufferDest = bitmap->getGraphicsBitmap();
    int size = canvas->width() * canvas->height();
    fastCopy((u32*) linearBuffer, (u32*) linearBufferDest, (u32) size);
}

void GcanvasPainter::setCanvasRect(Grect& rect) {
    cutRect = rect;
    cutRect &= *canvas;
}

GcanvasPainter::~GcanvasPainter() {

}