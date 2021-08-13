#ifndef CARDS_CHECK_H
#define CARDS_CHECK_H
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<string>
#include<cstring>
#include<map>
#include<vector>
#include<sstream>
#include "cards.h"
using std::string;
using std::map;
using std::to_string;
using std::vector;

class cards_check
{
public:
    int num[30],c[6];
    vector<Cards>cds;
    string cards_type;
    vector<Cards>cards_sequence;
    vector<Cards> val;

    cards_check();
    int go();
    bool chk_type();
    string number_to_info(int x);
    void init_num();
    int chk(vector<Cards>&cds_);

    void clear();
};

#endif // CARDS_CHECK_H
