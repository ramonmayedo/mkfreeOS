#include "unistd.h"
#include "stdio.h"
extern "C" {
    int kmain(int argc, char *argv[]);
    FILE *stdout, *stdin;
    void _start(int argc, char *argv[]) {
        stdin = fdopen(0, "r"); //fichero de entrada
        stdout = fdopen(1, "w"); //fichero de salida
        int error = kmain(argc, argv);
        exit(error);
    }
}

