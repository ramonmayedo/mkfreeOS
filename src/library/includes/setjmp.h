#ifndef SETJMP_H
#define SETJMP_H
#define u32 unsigned int 

struct __jmp_buf {
    u32 jebp;
    u32 jebx;
    u32 jedi;
    u32 jesi;
    u32 jesp;
    u32 ret;
    u32 exception;
    u32 context;
};

typedef struct __jmp_buf jmp_buf[1];
typedef struct __jmp_buf sigjmp_buf[1];

void longjmp(jmp_buf jmpb, int retval){
    
}

int setjmp(jmp_buf jmpb){
    return 0;
}

#endif

