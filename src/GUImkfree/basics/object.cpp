#include "object.h"

Gobject::Gobject() {

}

void Gobject::setClassName(string aclassName) {
    className = aclassName;
}

void Gobject::setClassName(char *aclassName) {
    className = aclassName;
}

string Gobject::getClassName() {
    return className;
}

bool Gobject::classNameIs(string fclassName) {
    return (className == fclassName);
}

Gobject::~Gobject() {
}

