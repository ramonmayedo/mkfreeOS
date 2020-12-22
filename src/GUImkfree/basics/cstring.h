#ifndef CSTRING_H
#define CSTRING_H

#include "vstring.h"

class string {
public:
    string();
    string(char *strchar);
    string(char *strchar, int count);
    string(const char onlyChar);
    string(const string &str);
    string(const int integer);
    string & operator=(string &otherstr);
    string & operator=(char *strchar);
    string & operator+=(char *strchar);
    string & operator+=(string & otherstr);
    string & operator+=(const int integer);
    string & operator+(string & otherstr);
    string & operator+(char *strchar);
    string & operator+(const int integer);
    char operator [](int index);
    bool operator==(string & otherstr);
    bool operator==(char *strchar);
    char *c_str();
    int insertChar(int index, char nchar);
    int deleteChar(int index);
    int deleteStrChar(int index, int count);
    int insertStringChar(int index, char *str, int count);
    int insertString(string *str);
    int Lenght();
    ~string();
private:
    Vstring *data;
};

#endif

