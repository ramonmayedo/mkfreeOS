#include "GgraphicComponent.h"
#include "Gwindows.h"
#include "../../library/includes/stdio.h"

GgraphicComponent::GgraphicComponent(int x, int y, int width, int height, GgraphicComponent *cparent) {
    realRect = Grect(x, y, width, height);
    childRect = realRect;
    position = Gpoint(x, y);
    font = new Gfont("a/kernel/uses/lucida.fnt");
    parent = cparent;
    if (parent)
        parent->insertChildComponent(this);
    focus = false;
    onClickDown = 0;
    onClickUp = 0;
    onPaint = 0;
    onMouseMove = 0;
    onKeyDown = 0;
    onFocused = 0;
    onResize = 0;
    componentAlign = caNone;
    componentPosition = cpRelative;
}

void GgraphicComponent::insertChildComponent(GgraphicComponent* childComponent) {
    int index = components.indexOf(childComponent);
    if (index != -1) return;
    childComponent->zindex = components.add(childComponent);
    childComponent->window = window;
    childComponent->canvas = canvas;
}

void GgraphicComponent::paint() {
    buildComponent();
    if (onPaint) onPaint(this, drawRect);
    GgraphicComponent *childComponent;
    for (int i = 0; i < components.count(); i++) {
        childComponent = (GgraphicComponent*) components.getItem(i);
        childComponent->paint();
    }
}

void GgraphicComponent::refresh() {
    window->updateZone(drawRect);
    ((Gwindow*) window)->flushRect(drawRect);
}

void GgraphicComponent::refresh(Grect rect) {
    rect.move(childRect);
    window->updateZone(rect);
    ((Gwindow*) window)->flushRect(rect);
}

GgraphicComponent* GgraphicComponent::getComponent(GgraphicComponent* component, int x, int y) {
    GgraphicComponent *selectedComponent = 0, *aux;
    int index = 0;
    for (int i = 0; i < component->components.count(); i++) {
        aux = (GgraphicComponent*) component->components.getItem(i);
        if (aux->drawRect.constains(x, y) && (aux->zindex > index) /*&& aux->visible*/) {
            index = aux->zindex;
            selectedComponent = aux;
        }
    }
    if (selectedComponent) {
        if (selectedComponent->components.count())
            selectedComponent = getComponent(selectedComponent, x, y);
        return selectedComponent;
    }
    return component;
}



void GgraphicComponent::buildComponent() {
    if (parent) {
        switch (componentAlign) {
            case caNone:
            {
                break;
            }
            case caRight:
            {
                realRect.setheight(parent->childRect.height());
                realRect.alignRight(parent->childRect);
                break;
            }
            case caBottom:
            {
                realRect.setwidth(parent->childRect.width());
                realRect.alignBottom(parent->childRect);
                break;
            }
        }
        drawRect = realRect;
        drawRect.move(parent->childRect);
        if (componentPosition == cpRelative)
            drawRect.move(parent->offsetPosition);
        position = drawRect;
        drawRect &= parent->childRect;
        childRect = drawRect;

        switch (componentAlign) {
            case caRight:
            {
                parent->childRect.reducedN(-drawRect.width(), 0);
                break;
            }
            case caBottom:
            {
                parent->childRect.reducedN(0, -drawRect.height());
                break;
            }
        }
    }
}

void GgraphicComponent::rebuildComponents() {
    GgraphicComponent *child;
    buildComponent();
    for (int i = 0; i < components.count(); i++) {
        child = (GgraphicComponent*) components.getItem(i);
        child->rebuildComponents();
    }
}

void GgraphicComponent::resizeComponent() {
    buildComponent();
    if (onResize) onResize(this);
    if (onPaint) onPaint(this, drawRect);
    GgraphicComponent *childComponent;
    for (int i = 0; i < components.count(); i++) {
        childComponent = (GgraphicComponent*) components.getItem(i);
        childComponent->resizeComponent();
    }
}

GgraphicComponent::~GgraphicComponent() {

}


void GgraphicComponent::updateZone(Grect& rect) {
    GgraphicComponent *child;
    for (int i = 0; i < components.count(); i++) {
        child = (GgraphicComponent*) components.getItem(i);
        if (rect.intersects(child->drawRect)) {
            Grect rectInt = rect;
            rectInt &= child->drawRect;
            if (child->onPaint)
                child->onPaint(child, rectInt);
            child->updateZone(rectInt);
        }
    }
}
