#ifndef CARDS_H
#define CARDS_H
#include<string>
#include<map>
using std::string;
using std::map;

class Cards
{
public:
    string cardid,info;
    int x,y,w,h;
    bool lift;

    Cards(string s);
    Cards(const char* s);

    friend bool operator < (Cards a, Cards b);
    void set(int x_,int y_,int w_,int h_);
    void clear();
};

#endif // CARDS_H
