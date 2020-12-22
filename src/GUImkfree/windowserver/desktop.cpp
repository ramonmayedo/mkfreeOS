#include "desktop.h"
#include "../basics/png.h"

Sdesktop::Sdesktop(Grect rect) : Sframe(rect) {
    frameType = ftDesktop;
}

void Sdesktop::loadWallpaper() {
    GgraphicsBitmap *pngBack = Gpng().loadPNG("a/kernel/uses/aquablue.png");
    if (pngBack)
        painter->drawBitmap(pngBack, *pngBack, 0, 0);
}

Sdesktop::~Sdesktop() {
}


