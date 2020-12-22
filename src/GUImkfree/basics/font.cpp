#include "font.h"
#include "../../library/includes/stdio.h"

Gfont::Gfont() {
    loadFont("a/kernel/uses/lucida.fnt");
}

Gfont::Gfont(char *dir) {
    loadFont(dir);
}

int Gfont::buildFontTypeBinary(char *buffer) {
    linearBuffer = new int[height * countChar];
    if (byteXwidth == 1) {
        for (int i = 0; i < height * countChar; i++)
            linearBuffer[i] = buffer[i];
        return 0;
    }
    if (byteXwidth == 2) {
        u16 *buffer16 = (u16*)buffer;
        for (int i = 0; i < height * countChar; i++)
            linearBuffer[i] = buffer16[i];
        return 0;
    }
}

int Gfont::loadFont(char* path) {
    FILE *fileFont = fopen(path, "r");
    SfontHeader header;
    fread(&header, sizeof (SfontHeader), 1, fileFont);
    fseek(fileFont, 256, SEEK_SET);
    if (fileFont) {
        width = header.width;
        height = header.height;
        countChar = header.countChar;
        byteXwidth = (width / 8);
        if (header.family == 0x24) {
            char *buffer;
            buffer = new char[height * byteXwidth * countChar];
            fread(buffer, height * countChar * byteXwidth, 1, fileFont);
            buildFontTypeBinary(buffer);
            spaceSeparator = 2;
            delete(buffer);
        }
        fclose(fileFont);
    }
}