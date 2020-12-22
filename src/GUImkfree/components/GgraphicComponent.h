#ifndef GGRAPHICCOMPONENT_H
#define GGRAPHICCOMPONENT_H
#include "../basics/grect.h"
#include "../basics/list.h"
#include "../basics/font.h"
#include "../basics/canvasPainter.h"

class GgraphicComponent;
class Gwindow;
typedef void (eventOnPaint) (GgraphicComponent* sender, Grect &rect);
typedef void (eventOnResize) (GgraphicComponent* sender);
typedef void (eventOnClick) (GgraphicComponent* sender, int x, int y);
typedef void (eventOnMouseMove) (GgraphicComponent* sender, int x, int y);
typedef void (eventOnKeyPress) (GgraphicComponent* sender, int &key);
typedef void (eventOnFocused) (GgraphicComponent* sender);

enum eComponentAlign {
    caNone, caRight, caLeft, caTop, caBottom
};
enum eComponentPosition{
    cpFixed, cpRelative
};
class GgraphicComponent {
public:
    GgraphicComponent(int x, int y, int width, int height, GgraphicComponent *cparent);
    void paint();
    void repaint();
    void refresh();
    void updateZone(Grect &rect);
    void refresh(Grect rect);
    void resize();
    void insertChildComponent(GgraphicComponent *childComponent);
    void paintZone(Grect &rect, int index);
    GgraphicComponent* getComponent(GgraphicComponent* component, int x, int y);
    void buildComponent();
    void rebuildComponents();
    void resizeComponent();
    eventOnClick *onClickDown;
    eventOnClick *onClickUp;
    eventOnPaint *onPaint;
    eventOnMouseMove *onMouseMove;
    eventOnKeyPress *onKeyDown;
    eventOnFocused *onFocused;
    eventOnResize *onResize;
    Grect drawRect, realRect, childRect;
    Gpoint position, offsetPosition;
    GgraphicComponent *window;
    eComponentAlign componentAlign; 
    eComponentPosition componentPosition;
    Gfont *font;
    bool focus;
    int zindex;
    GcanvasPainter *canvas;
    Clist components;
    GgraphicComponent *parent;
    ~GgraphicComponent();
private:
    
};

#endif

