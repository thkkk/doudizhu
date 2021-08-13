#include "cards.h"

Cards::Cards(string s)
{
    cardid = s;
    if(cardid[0]=='C' || cardid[0]=='D' || cardid[0]=='H' || cardid[0]=='S')
    {
        info =cardid.substr(1,cardid.length()-1);
    }
    else
    {
        info = cardid;
    }
    clear();
}

Cards::Cards(const char* s)
{
    cardid = s;
    if(cardid[0]=='C' || cardid[0]=='D' || cardid[0]=='H' || cardid[0]=='S')
    {
        info =cardid.substr(1,cardid.length()-1);
    }
    else
    {
        info = cardid;
    }
    clear();
}

void Cards::clear()
{
    x=y=w=h=0;
    lift=0;
}

bool operator < (Cards a, Cards b)
{
    static map<string,int> mp;
    if(mp.empty())
    {
        mp["3"]=3;
        mp["4"]=4;
        mp["5"]=5;
        mp["6"]=6;
        mp["7"]=7;
        mp["8"]=8;
        mp["9"]=9;
        mp["10"]=10;
        mp["J"]=11;
        mp["Q"]=12;
        mp["K"]=13;
        mp["A"]=14;
        mp["2"]=15;
        mp["BLACKJOKER"]=16;
        mp["REDJOKER"]=17;
    }
    return mp[a.info]<mp[b.info];
}


void Cards::set(int x_,int y_,int w_,int h_)
{
    x = x_;
    y = y_;
    w = w_;
    h = h_;
}
