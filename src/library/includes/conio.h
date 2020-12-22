#ifndef CONIO_H
#define CONIO_H

#define CAR_JMPLINE       0x0A
#define CAR_RETCURSOR     0x0D
#define CAR_ATTRIBUTE     0x07
#define CAR_BACKSPACE     0x08

struct SvideoArea {
    int *area;
    int left;
    int top;
    int height;
    int width;
    int trueLeft, trueTop, trueHeight, trueWidth;
};

enum MODESVIDEO {
    mvText, mvGraphics
};

enum EmodeVideo {
    t80x25x16=1,g320x200x256=2
};

void textcolor(int newcolor);
void gotoxy(int posX, int posY);
int wherex();
int wherey();
int blockDisplay();
int unblockDisplay();
int changeMode(int mode);
int changeModeAdvance(int mode,int width,int height);
int paintArea(SvideoArea *area);
int getMouseState(int *dx,int *dy, int *dz);
int getKeyPress();
int getArea(SvideoArea *area);



#endif

