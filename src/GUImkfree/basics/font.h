#ifndef FONT_H
#define FONT_H

#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char

struct SfontHeader {
    u32 typeFile; //Identificador de los archivos mkfreeOS
    u8 magicChar[6]; //MKFONT
    u32 family;
    u8 width;
    u8 height;
    u32 countChar;
    char name[128];
};

enum TextAlignment {
    talignTopLeft,
    talignCenterTop,
    talignCenter,
    talignCenterBottom,
    talignTopRight,
    talignNone,
};

class Gfont {
public:
    Gfont();
    Gfont(char *dir);
    int width;
    int height;
    int spaceSeparator;
    int countChar;
    int *linearBuffer;
private:
    int buildFontTypeBinary(char *buffer);
    int loadFont(char *path);
    int byteXwidth;
};

#endif

