#ifndef gsizeH
#define gsizeH

class Grect;

class Gsize {
    friend Grect;
public:
    Gsize();
    Gsize(int width, int height);
    int width();
    int height();
    void setwidth(int width);
    void setheight(int height);
    bool operator == (Gsize &size);
    Gsize operator-(Gsize &size);
    Gsize operator+(Gsize &size);
    Gsize &operator+=(Gsize &size);
    Gsize &operator-=(Gsize &size);
private:
    int swidth, sheight;
};

#endif
