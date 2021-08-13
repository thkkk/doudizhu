#ifndef PLAYED_CARDS_H
#define PLAYED_CARDS_H
#include"cards.h"
#include<string>
#include<vector>
using std::vector;
using std::string;

class Played_cards
{
public:
    Played_cards();
    string cards_type;
    vector<Cards>cards_sequence;
    vector<Cards> val;

    void clear();
    bool greater_comp(Played_cards& b);
    bool empty();
};

#endif // PLAYED_CARDS_H
