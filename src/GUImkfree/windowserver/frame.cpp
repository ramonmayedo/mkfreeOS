#include "frame.h"
#include "windowserver.h"
#include "../../library/includes/unistd.h"

extern SwindowServer * windowServer;

Sframe::Sframe(Grect rect) {
    frameType = ftNone;
    drawRect = rect;
    frameRect = Grect(0, 0, rect.width(), rect.height());
    painter = new GcanvasPainter(new GgraphicsBitmap(rect.width(), rect.height()));
    keyShared = 0;
}

Sframe::Sframe(Grect rect, Gsize size) {
    frameType = ftNone;
    drawRect = rect;
    frameRect = Grect(0, 0, rect.width(), rect.height());
    int linearBuffer; // = new int[size.width() * size.height()] ;
    keyShared = shmem(size.width() * size.height() * 4, &linearBuffer);
    painter = new GcanvasPainter(new GgraphicsBitmap(rect.width(), rect.height(), (int*) linearBuffer));
}

void Sframe::refreshRect(Grect& rect) {
    windowServer->composer->drawRectDirty(this, rect);
}

Sframe::~Sframe() {
}

