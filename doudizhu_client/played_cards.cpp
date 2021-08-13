#include "played_cards.h"

Played_cards::Played_cards()
{
    clear();
}

void Played_cards::clear()
{
    cards_type .clear();
    cards_sequence.clear();
    val.clear();
}

bool Played_cards::greater_comp(Played_cards &b)
{
    if(cards_type != b.cards_type)
    {
        if(cards_type == "bomb")
        {
            if(b.cards_type=="bomb" && b.val[0] < val[0] ) return 1;
            else if(b.cards_type!="bomb") return 1;
            return 0;
        }
        return 0;
    }
    if(val.size() != b.val.size()) return 0;
    for(int i=0,sz=val.size();i<sz;++i)
    {
        if(b.val[i] < val[i]) continue;
        else return 0;
    }
    return 1;
}

bool Played_cards::empty()
{
    return cards_sequence.empty();
}
