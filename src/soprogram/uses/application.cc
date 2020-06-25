#include "application.h"
#include "../../library/includes/conio.h"

Capplication::Capplication(char *aname, char *afileName, int aargc) {
    name = aname;
    argc = aargc;
    argv = new char*[argc];
    fileName = afileName;
    argv[0] = name;
    argcount = 1;
}

Capplication::Capplication() {

}

int Capplication::execute() {
    argv[argcount] = 0x0;
    return execv(fileName, argv);
}

int Capplication::addArg(char *aarg) {
    argv[argcount] = aarg;
    argcount++;
}

void Capplication::blockDisplay() {

}

void Capplication::deleteArg() {
    while (argcount--) delete(argv[argcount]);
}