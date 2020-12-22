#include "cstring.h"
#include "../../library/includes/string.h"
#include "../../library/includes/conversion.h"

string::string() {
    data = new Vstring(1, char(0x0));
}

string::string(char* strchar) {
    int sizeStr = strlen(strchar);
    data = new Vstring(sizeStr, strchar);
    data->insertLast(char(0x0));
}

string::string(char* strchar, int count) {
    data = new Vstring(count, strchar);
    data->insertLast(char(0x0));
}

string::string(const char onlyChar) {
    data = new Vstring(1, onlyChar);
    data->insertLast(char(0x0));
}

string::string(const string &str) {
    data = new Vstring(*str.data);
}

string::string(const int integer) {
    char dataConv[10];
    int size = IntToStrChar(integer, dataConv);
    data = new Vstring(size);
    data->append(dataConv, size);
}

string& string::operator=(char* strchar) {
    data->clear();
    data->append(strchar, strlen(strchar));
    data->insertLast(char(0x0));
    return *this;
}

string& string::operator=(string& otherstr) {
    if (otherstr == *this) return *this;
    data->clear();
    data->append(*otherstr.data);
    return *this;
}

string& string::operator+=(char* strchar) {
    data->removeLast();
    data->append(strchar, strlen(strchar));
    char end = 0x0;
    data->insertLast(end);
    return *this;
}

string& string::operator+=(string& otherstr) {
    data->removeLast();
    data->append(*otherstr.data);
    return *this;
}

string & string::operator+=(const int integer) {
    char dataConv[10];
    IntToStrChar(integer, dataConv);
    operator +=(dataConv);
    return *this;
}

string& string::operator+(string& otherstr) {
    return operator +=(otherstr);
}

string& string::operator+(char * otherstr) {
    return operator +=(otherstr);
}

string& string::operator+(const int integer) {
    char dataConv[10];
    IntToStrChar(integer, dataConv);
    return operator +=(dataConv);
}

char string::operator[](int index) {
    if (data->count() > index) {
        return data->operator[](index);
    }
    return 0x0;
}

bool string::operator==(string& otherstr) {
    return !strcmp(c_str(), otherstr.c_str());
}

bool string::operator==(char* strchar) {
    return !strcmp(c_str(), strchar);
}

int string::Lenght() {
    return data->count();
}

char* string::c_str() {
    return data->data();
}

int string::insertChar(int index, char nchar) {
    data->insert(index, nchar);
}

int string::deleteChar(int index) {
    data->remove(index);
}

int string::deleteStrChar(int index, int count) {
    data->remove(index, count);
}

int string::insertStringChar(int index, char* str, int count) {
    data->insert(index, str, count);
}

int string::insertString(string *str) {
    data->removeLast();
    data->append(*(str->data));
}

string::~string() {
    data->clear();
    delete(data);
}