#include "vstring.h"

Vstring::Vstring(int n) {
    intialized(n);
}

Vstring::Vstring(int n, char data) {
    intialized(n);
    for (int i = 0; i < n; i++) vstart[i] = data;
}

Vstring::Vstring(int n, char* data) {
    intialized(n);
    for (int i = 0; i < n; i++) vstart[i] = data[i];
}

Vstring::Vstring(Vstring& vector) {
    intialized(vector.vsize);
    for (int i = 0; i < vector.vsize; i++) vstart[i] = vector.vstart[i];
}

void Vstring::intialized(int n) {
    if (n < 255) {
        vstart = new char[255];
        vcapacity = 255;
    } else {
        vstart = new char[n];
        vcapacity = n;
    }
    vsize = n;
    vindex = 0;
}

char Vstring::operator[](int index) {
    if (index >= 0 && index < vsize)
        return vstart[index];
}

char Vstring::first() {
    return vstart[0];
}

char Vstring::last() {
    return vstart[vsize - 1];
}

char Vstring::remove(int index) {
    if (index < 0 || index >= vsize) return vstart[0];
    //ShowMessage(index) ;
    char tempValue = vstart[index];
    for (int i = index + 1; i < vsize; i++) vstart[i - 1] = vstart[i];
    vsize--;
    return tempValue;
}

void Vstring::remove(int index, int count) {
    if (index < 0 || (index + count) >= vsize) return;
     vsize -= count;
    for (int i = index; i < vsize; i++) vstart[i] = vstart[i+count];

}

char Vstring::removeFirst() {
    return remove(0);
}

char Vstring::removeLast() {
    return remove(vsize - 1);
}

void Vstring::needCapacity(int newCapacity) {
    if (vcapacity > newCapacity) return;
    char *newStart = new char[newCapacity];
    for (int i = 0; i < newCapacity; i++) newStart[i] = vstart[i];
    delete(vstart);
    vstart = newStart;
    vcapacity = newCapacity;
}

void Vstring::insert(int index, char value) {
    if (index > vsize || index < 0) return;
    if (index == vsize) return append(value);
    needCapacity(vsize + 1);
    vsize++;
    for (int i = vsize - 1; i > index; i--) vstart[i] = vstart[i - 1];
    vstart[index] = value;
}

void Vstring::insert(int index, char* values, int count) {
    if (index > vsize || index < 0) return;
    if (index == vsize) return append(values, count);
    needCapacity(vsize + count);
    for (int i = vsize + count - 1; i > index; i--) vstart[i] = vstart[i - 1];
    for (int i = index, j = 0; i < count; i++, j++) vstart[i] = values[j];
    vsize+=count;
}

void Vstring::insertFirst(char value) {
    insert(0, value);
}

void Vstring::insertLast(char value) {
    insert(vsize, value);
}

void Vstring::append(Vstring& vector) {
    needCapacity(vsize + vector.count());
    //ShowMessage(vector.count());
    for (int i = 0; i < vector.count(); i++) vstart[vsize + i] = vector.vstart[i];
    vsize+=vector.count();
}

void Vstring::append(char value) {
    needCapacity(vsize + 1);
    vstart[vsize] = value;
    vsize++;
}

void Vstring::append(char* values, int count) {
    if (count == 0) return;
    needCapacity(vsize + count);
    for (int i = 0; i < count; i++) vstart[vsize + i] = values[i];
    vsize += count;
}

void Vstring::clear() {
    vsize = 0;
}

int Vstring::count() {
    return vsize;
}

bool Vstring::emty() {
    return (vsize == 0);
}

char* Vstring::data() {
    return vstart;
}

void Vstring::setSize(int newSize) {
    needCapacity(newSize);
    vsize = newSize;
}
