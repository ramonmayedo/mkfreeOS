#ifndef gpointH
#define gpointH

class Grect;

class Gpoint {
    friend Grect;
public:
    Gpoint();
    Gpoint(int x, int y);
    int x();
    int y();
    void setx(int x);
    void sety(int y);
    void setxy(int x, int y);
    void move(int dx,int dy);
    void move(Gpoint &dpoint);
    Gpoint traslate(int dx,int dy);
    Gpoint traslate(Gpoint &point);
    bool operator == (Gpoint &point);
    Gpoint operator-();
    Gpoint operator-(Gpoint &point);
    Gpoint operator+(Gpoint &point);
    Gpoint &operator+=(Gpoint &point);
    Gpoint &operator-=(Gpoint &point);
private:
    int px, py;
};
//---------------------------------------------------------------------------
#endif
