#ifndef CONIO_H
#define CONIO_H

#define CAR_JMPLINE       0x0A
#define CAR_RETCURSOR     0x0D
#define CAR_ATTRIBUTE     0x07
#define CAR_BACKSPACE     0x08

enum commandConsole {
    cmcColor = 1, cmcGotoXY = 2, cmcWhereX = 3, cmcWhereY = 4,
    cmcPrintf = 5, cmcBlock = 6, cmcUnBlock = 7, cmcGetChar = 8,
    cmcPutChar = 9, cmcReadChar = 10, cmcWrite = 11
};

enum commandGraphics {
    cmgChangeModeVideo = 1, cmgPaintArea = 2, cgpChangeModeVideoAdvanced = 3,
    cmpGetArea = 4
};

enum commandMouse {
    cmmMouseEvent = 1
};

enum eCommandKeyboard {
    cmkKeyEvent = 1
};


enum COLORS {
    clBLACK, clRED, clGREEN, clCYAN, clBLUE, clMAGENTA,
    clBROWN, clLIGHTGRAY, clDARKGRAY, clLIGHTBLUE,
    clLIGHTGREEN, clLIGHTCYAN, clLIGHTRED, clLIGHTMAGENTA,
    clYELLOW, clWHITE
};

struct SvideoArea {
    int *area;
    int left;
    int top;
    int height;
    int width;
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

