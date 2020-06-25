#ifndef GPROGRAM_H
#define GPROGRAM_H
#include "unistd.h"

int main(int argc, char *argv[]);

void _start(int argc, char *argv[]) {
    int error = main(argc, argv);
    exit(error);
}


#endif

