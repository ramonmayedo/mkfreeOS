#include "list.h"

Clist::Clist() {
    firstNode = 0;
    lastNode = 0;
    ncount = 0;
}

void Clist::initialize() {
    firstNode = 0;
    lastNode = 0;
    ncount = 0;
}

int Clist::add(void * aitem) {
    Cnode *newNode = new Cnode(aitem);
    if (ncount == 0) {
        firstNode = newNode;
        lastNode = newNode;
    } else {
        newNode->setPrev(lastNode);
        lastNode->setNext(newNode);
        lastNode = newNode;
    }
    ncount++;
    return ncount;
}

Clist::~Clist() {
    clear();
}

void *Clist::getFirst() {
    return firstNode->getData();
}

void *Clist::getLast() {
    return lastNode->getData();
}

int Clist::count() {
    return ncount;
}

Cnode *Clist::getNode(int aindex) {
    Cnode *item = firstNode;
    for (int i = 0; i < aindex; i++)
        item = item->getNext();
    return item;
}

void *Clist::getItem(int aindex) {
    if (ncount == 0 || aindex >= ncount) return 0;
    return getNode(aindex)->getData();
}

void Clist::setItem(int aindex, void *aitem) {
    if (ncount == 0 || aindex >= ncount) return;
    getNode(aindex)->setData(aitem);
}

bool Clist::isEmty() {
    if (ncount == 0) return true;
    return false;
}

void Clist::clear() {
    if (ncount == 0) return;
    Cnode *item = firstNode, *auxItem;
    for (int i = 0; i < ncount; i++) {
        auxItem = item;
        item = item->getNext();
        delete(auxItem);
    }
    ncount = 0;
}

void Clist::move(int aindex1, int aindex2) {
    if (aindex1 == aindex2) return;
    if (aindex1 >= ncount || aindex2 >= ncount) return;
    Cnode *item1 = getNode(aindex1), *item2 = getNode(aindex2);
    void *auxData = item1->getData();
    item1->setData(item2->getData());
    item2->setData(auxData);
}

void Clist::delet(int aindex) {
    if (ncount == 0 || aindex >= ncount) return;
    Cnode *item = getNode(aindex);
    if (ncount > 1) {
        if (aindex == 0) {
            firstNode = item->getNext();
            firstNode->setPrev(0);
        } else if (aindex == ncount - 1) {
            lastNode = item->getPrev();
            lastNode->setNext(0);
        } else {
            item->getPrev()->setNext(item->getNext());
            item->getNext()->setPrev(item->getPrev());
        }
    }
    ncount--;
   delete(item);
}

void Clist::insert(int aindex, void *aitem) {
    if (ncount == 0) {
        add(aitem);
        return;
    }
    if (aindex >= ncount) return;
    Cnode *newNode = new Cnode(aitem), *node = getNode(aindex);
    newNode->setNext(node);
    if (node->getPrev() != 0) {
        node->getPrev()->setNext(newNode);
        newNode->setPrev(node->getPrev());
    } else firstNode = newNode;
    node->setPrev(newNode);
    ncount++;
}

int Clist::indexOf(void *aitem) {
    Cnode *item = firstNode;
    for (int i = 0; i < ncount; i++) {
        if (item->getData() == aitem) return i;
        item = item->getNext();
    }
    return -1;
}

int Clist::remove(void *aitem) {
    int index = indexOf(aitem);
    delet(index);
    return index;
}
void*  Clist::removeLast(){
    void *last =  lastNode;
    delet(ncount-1);
    return last;
}

void *Clist::removeFirst() {
    Cnode *first =  firstNode;
    delet(0);
    return first->getData();
}