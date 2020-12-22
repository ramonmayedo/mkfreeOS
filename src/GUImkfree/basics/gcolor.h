//---------------------------------------------------------------------------

#ifndef gcolorH
#define gcolorH
class Gcolor {
public:
    Gcolor();
    Gcolor(int color);
    Gcolor(unsigned char r,unsigned char g, unsigned char b);
    Gcolor(unsigned char r, unsigned char g, unsigned char b, char a);
    void setcolor(int color);
    int valColor();
    unsigned char red();
    void setred(char r);
    unsigned char green();
    void setgreen(char g);
    unsigned char blue();
    void setblue(char b);
    unsigned char alpha();
    void setalpha(char a);
    Gcolor widthAlpha(char a);
    Gcolor grayScale();
    Gcolor darkned();
    Gcolor lightned();
    Gcolor inverted();
    bool operator == (Gcolor &color);
    bool operator != (Gcolor &color);
private:
    int  vcolor;
};
//---------------------------------------------------------------------------
#endif
