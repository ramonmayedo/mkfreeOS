#ifndef gbitmapH
#define gbitmapH

#include "grect.h"
#include "gcolor.h"
#include "gcolor.h"
#include "font.h"

class GgraphicsBitmap : public  Grect {
public:
    GgraphicsBitmap(int width, int height);
    GgraphicsBitmap(Gfont *font,Gcolor bkColor, Gcolor fnColor, const char car);
    GgraphicsBitmap(Grect &rect, Gfont *font,Gcolor bkColor, Gcolor fnColor, const char car);
    GgraphicsBitmap(int width, int height, int *externGraphicsBitmap);
    int *getLine(int y);
    int *getGraphicsBitmap();
    void setGraphicsBitmap(int *externGraphicsBitmap);
    void resizeBitmap(int newWidth,int newHeight);
    ~GgraphicsBitmap();
private:
    int *glinearBuffer;
    bool isExternGrpahics;
};
#endif
