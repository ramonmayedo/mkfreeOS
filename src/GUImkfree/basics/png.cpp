#include "png.h"
#include "../../library/includes/memory.h"
#include "../../library/includes/string.h"
#include "../../library/includes/stdio.h"
#include "puff.h"
#include "canvasPainter.h"
#include "templates.h"

#define ABS(a) ((a >= 0) ? (a) : -(a))

Gpng::Gpng() {
}

GgraphicsBitmap* Gpng::loadPNG(char* fileName) {
    char header[8] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
    char *bufferHead = new char[8];
    char crc[4];
    decodeState.file = fopen(fileName, "r");
    if (decodeState.file == 0) return 0;
    fread(bufferHead, sizeof (header), 1, decodeState.file);
    if (memcmp(header, bufferHead, sizeof (header)) != 0) return 0;
    SheaderChunk chunkData;
    int max = 10;
    while (max > 0) {
        fread(&chunkData, sizeof (SheaderChunk), 1, decodeState.file);
        int size = littToBig(chunkData.size);
        if (memcmp(chunkData.name, "IHDR", sizeof (chunkData.name)) == 0) {
            if (!processIHDR()) return 0;
            fread(crc, sizeof (crc), 1, decodeState.file);
        } else if (memcmp(chunkData.name, "IDAT", sizeof (chunkData.name)) == 0) {
            char *buffer = new char[size];
            fread(buffer, size, 1, decodeState.file);
            decode(buffer, size);
            delete(buffer);
            fread(crc, sizeof (crc), 1, decodeState.file);
            break;
        } else
            fseek(decodeState.file, size + 4, SEEK_CUR);

        if (size == 0) break;
        max--;
    }
    return bitmap;
}

bool Gpng::processIHDR() {
    fread(&decodeState.chunkIHDR, sizeof (decodeState.chunkIHDR), 1, decodeState.file);
    decodeState.chunkIHDR.width = littToBig(decodeState.chunkIHDR.width);
    decodeState.chunkIHDR.height = littToBig(decodeState.chunkIHDR.height);
    if (decodeState.chunkIHDR.InterlaceMethod != 0) return false;
    switch (decodeState.chunkIHDR.colorType) {
        case 6:
        {
            decodeState.channels = 4;
            break;
        }
        case 2:
        {
            decodeState.channels = 3;
            break;
        }
        default:
        {
           // printf("No soportado Profundidad de bit %i", decodeState.chunkIHDR.colorType);
            while (1);
            return false;
        }
    }
    return true;
}

bool Gpng::decode(char* buffer, int size) {
    u8 *bufferData = (u8*) buffer + 2;
    unsigned long sizeBuffDesc = (decodeState.chunkIHDR.height + 1) * decodeState.chunkIHDR.width * decodeState.channels;
    u8 *buffDescompresion = new u8[sizeBuffDesc];
    unsigned long sizeBuffData = size - 6;
    int error = puff(buffDescompresion, &sizeBuffDesc, bufferData, &sizeBuffData);
    if (error) {
       // printf("Error en la descompresion %i ", error);
        delete(buffDescompresion);
        while (1);
        return false;
    }
    bitmap = new GgraphicsBitmap(decodeState.chunkIHDR.width, decodeState.chunkIHDR.height);
    if (decodeState.channels == 4)
        decode32(buffDescompresion);
    else if (decodeState.channels == 3)
        decode24(buffDescompresion);

    delete(buffDescompresion);
    return true;
}

bool Gpng::decode32(u8* buffer) {
    Scolor32 *copyBuffer;
    char *bufferInt = (char*) buffer;
    Scolor32 *color;
    Scolor32 blankColor;
    blankColor.r = 0;
    blankColor.g = 0;
    blankColor.b = 0;
    for (int i = 0; i < decodeState.chunkIHDR.height; i++) {
        int tipo = bufferInt[0];
        bufferInt++;
        color = (Scolor32*) bitmap->getLine(i);
        copyBuffer = (Scolor32*) bufferInt;
        if (tipo == 0) {
            for (int j = 0; j < decodeState.chunkIHDR.width; j++) {
                color[j].r = copyBuffer[j].b;
                color[j].g = copyBuffer[j].g;
                color[j].b = copyBuffer[j].r;
                color[j].a = copyBuffer[j].a;
            }
        } else if (tipo == 1) {
            Scolor32 left = blankColor;
            color[0].r = copyBuffer[0].b;
            color[0].g = copyBuffer[0].g;
            color[0].b = copyBuffer[0].r;
            color[0].a = copyBuffer[0].a;
            for (int j = 1; j < decodeState.chunkIHDR.width; j++) {
                left = color[j - 1];
                color[j].r = copyBuffer[j].b + left.r;
                color[j].g = copyBuffer[j].g + left.g;
                color[j].b = copyBuffer[j].r + left.b;
                color[j].a = copyBuffer[j].a + left.a;
            }
        } else if (tipo == 2) {
            Scolor32 *bcolor = (Scolor32*) bitmap->getLine(i - 1);
            for (int j = 0; j < decodeState.chunkIHDR.width; j++) {
                Scolor32 up = bcolor[j];
                color[j].r = copyBuffer[j].b + up.r;
                color[j].g = copyBuffer[j].g + up.g;
                color[j].b = copyBuffer[j].r + up.b;
                color[j].a = copyBuffer[j].a + up.a;
            }
        } else if (tipo == 3) {
            Scolor32 left = blankColor;
            Scolor32 up = blankColor;
            Scolor32 *bcolor = (Scolor32*) bitmap->getLine(i - 1);
            for (int j = 0; j < decodeState.chunkIHDR.width; j++) {
                if (j > 0)
                    left = color[j - 1];
                if (i > 0)
                    up = bcolor[j];
                color[j].r = copyBuffer[j].b + (left.r + up.r) / 2;
                color[j].g = copyBuffer[j].g + (left.g + up.g) / 2;
                color[j].b = copyBuffer[j].r + (left.b + up.b) / 2;
                color[j].a = copyBuffer[j].a + (left.a + up.a) / 2;
            }
        } else if (tipo == 4) {
            Scolor32 left = blankColor;
            Scolor32 up = blankColor;
            Scolor32 upleft = blankColor;
            Scolor32 *bcolor = (Scolor32*) bitmap->getLine(i - 1);
            for (int j = 0; j < decodeState.chunkIHDR.width; j++) {
                if (j > 0)
                    left = color[j - 1];
                if (i > 0)
                    up = bcolor[j];
                if (j > 0 && i > 0)
                    upleft = bcolor[j - 1];
                color[j].r = copyBuffer[j].b + paeth(left.r, up.r, upleft.r);
                color[j].g = copyBuffer[j].g + paeth(left.g, up.g, upleft.g);
                color[j].b = copyBuffer[j].r + paeth(left.b, up.b, upleft.b);
                color[j].a = copyBuffer[j].a + paeth(left.a, up.a, upleft.a);
            }
        } else {
           // printf("Tipo de Filtro Desconocido %i ", tipo);
            while (1);
            return false;
        }
        bufferInt = bufferInt + decodeState.chunkIHDR.width * decodeState.channels;
    }
}

bool Gpng::decode24(u8* buffer) {
    Scolor24 *copyBuffer;
    char *bufferInt = (char*) buffer;
    Scolor32 *color;
    Scolor32 blankColor;
    blankColor.r = 0;
    blankColor.g = 0;
    blankColor.b = 0;
    for (int i = 0; i < decodeState.chunkIHDR.height; i++) {
        int tipo = bufferInt[0];
        bufferInt++;
        color = (Scolor32*) bitmap->getLine(i);
        copyBuffer = (Scolor24*) bufferInt;
        if (tipo == 0) {
            for (int j = 0; j < decodeState.chunkIHDR.width; j++) {
                color[j].r = copyBuffer[j].b;
                color[j].g = copyBuffer[j].g;
                color[j].b = copyBuffer[j].r;
            }
        } else if (tipo == 1) {
            Scolor32 left = blankColor;
            color[0].r = copyBuffer[0].b;
            color[0].g = copyBuffer[0].g;
            color[0].b = copyBuffer[0].r;
            for (int j = 1; j < decodeState.chunkIHDR.width; j++) {
                left = color[j - 1];
                color[j].r = copyBuffer[j].b + left.r;
                color[j].g = copyBuffer[j].g + left.g;
                color[j].b = copyBuffer[j].r + left.b;
            }
        } else if (tipo == 2) {
            Scolor32 *bcolor = (Scolor32*) bitmap->getLine(i - 1);
            for (int j = 0; j < decodeState.chunkIHDR.width; j++) {
                Scolor32 up = bcolor[j];
                color[j].r = copyBuffer[j].b + up.r;
                color[j].g = copyBuffer[j].g + up.g;
                color[j].b = copyBuffer[j].r + up.b;
            }
        } else if (tipo == 3) {
            Scolor32 left = blankColor;
            Scolor32 up = blankColor;
            Scolor32 *bcolor = (Scolor32*) bitmap->getLine(i - 1);
            for (int j = 0; j < decodeState.chunkIHDR.width; j++) {
                if (j > 0)
                    left = color[j - 1];
                if (i > 0)
                    up = bcolor[j];
                color[j].r = copyBuffer[j].b + (left.r + up.r) / 2;
                color[j].g = copyBuffer[j].g + (left.g + up.g) / 2;
                color[j].b = copyBuffer[j].r + (left.b + up.b) / 2;
            }
        } else if (tipo == 4) {
            Scolor32 left = blankColor;
            Scolor32 up = blankColor;
            Scolor32 upleft = blankColor;
            Scolor32 *bcolor = (Scolor32*) bitmap->getLine(i - 1);
            for (int j = 0; j < decodeState.chunkIHDR.width; j++) {
                if (j > 0)
                    left = color[j - 1];
                if (i > 0)
                    up = bcolor[j];
                if (j > 0 && i > 0)
                    upleft = bcolor[j - 1];
                color[j].r = copyBuffer[j].b + paeth(left.r, up.r, upleft.r);
                color[j].g = copyBuffer[j].g + paeth(left.g, up.g, upleft.g);
                color[j].b = copyBuffer[j].r + paeth(left.b, up.b, upleft.b);
            }
        } else {
            //printf("Tipo de Filtro Desconocido %i ", tipo);
            while (1);
            return false;
        }
        bufferInt = bufferInt + decodeState.chunkIHDR.width * decodeState.channels;
    }
}

int Gpng::littToBig(int dataLitt) {
    return (((dataLitt & 0xFF000000) >> 24) | ((dataLitt & 0x00FF0000) >> 8) | ((dataLitt & 0x0000FF00) << 8) | ((dataLitt & 0x000000FF) << 24));
}

int Gpng::paeth(int a, int b, int c) {
    int p = a + b - c;
    int pa = ABS(p - a);
    int pb = ABS(p - b);
    int pc = ABS(p - c);
    if (pa <= pb && pa <= pc) return a;
    if (pb <= pc) return b;
    return c;
}

Gpng::~Gpng() {
}