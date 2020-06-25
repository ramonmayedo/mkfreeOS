#ifndef APPLICATION_H
#define APPLICATION_H

class Capplication {
public:
    Capplication(char *aname, char *afileName, int aargc);
    Capplication();
    int addArg(char *aarg);
    int execute();
    void blockDisplay();
    void deleteArg();
private:
    char *name;
    char *fileName;
    char **argv;
    int argc;
    int argcount;
};

#endif

