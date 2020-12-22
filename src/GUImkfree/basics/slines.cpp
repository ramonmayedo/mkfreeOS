#include "slines.h"

slines::slines() {
    lines = new Clist();
}

void slines::insert(int index, string* str) {
    if (index >= count())
        lines->add(str);
    else
        lines->insert(index, str);
}

void slines::add(string* str) {
    lines->add(str);
}

string* slines::operator[](int index) {
    return (string*) lines->getItem(index);
}

void slines::Delete(int index) {
    string *deleteString = (string*) lines->getItem(index);
    lines->delet(index);
    if (deleteString) delete(deleteString);
}

void slines::clear() {
    for (int i = 0; i < lines->count(); i++)
        delete((string*) lines->getItem(i));
    lines->clear();
}

int slines::count() {
    return lines->count();
}

slines::~slines() {
}

