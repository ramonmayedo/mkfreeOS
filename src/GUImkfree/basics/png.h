#ifndef PNG_H
#define PNG_H
#include "gbitmap.h"
#include "gcolor.h"
#include "../../library/includes/stdio.h"

#define u8  unsigned char
struct SchunkData {
    int size;
    char name[4];
    int filePTR;
}__attribute__((packed));;

struct SheaderChunk {
    int size;
    char name[4];
} __attribute__((packed));

struct SchunkIHDR {
    int width;
    int height;
    u8 bitDepth;
    u8 colorType;
    u8 compresionMethod;
    u8 filterMethod;
    u8 InterlaceMethod;
} __attribute__((packed));

struct SdecodeState {
    FILE *file;
    SchunkIHDR chunkIHDR;
    char channels;  
};

struct Scolor32 {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

struct Scolor24 {
    u8 r;
    u8 g;
    u8 b;
};

class Gpng {
public:
    Gpng();
    GgraphicsBitmap *loadPNG(char *fileName);
    int littToBig(int dataLitt);
    ~Gpng();
private:
    SdecodeState decodeState;
    bool decode(char *buffer, int size);
    bool processIHDR();
    GgraphicsBitmap *bitmap;
    int paeth(int a, int b, int c);
    bool decode32(u8 *buffer);
    bool decode24(u8 *buffer);
};

#endif

