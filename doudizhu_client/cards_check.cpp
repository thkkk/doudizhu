#include "cards_check.h"

cards_check::cards_check()
{
    clear();
}

int cards_check::go()
{
    memset(c,0,sizeof(c));
    int tot=0;
    for(int i=3;i<=16;++i) c[num[i]]++;
    while(c[4]&&c[1]>=2)
    {
        c[4]--,c[1]-=2,tot++;
        cards_type = "four_two";
    }
    while(c[4]&&c[2]>=2)
    {
        c[4]--,c[2]-=2,tot++;
        cards_type = "four_two_pair";
    }
    while(c[4]&&c[1])
    {
        c[4]--,c[1]--,tot++;
        cards_type = "four_one";
    }
    while(c[4]&&c[2])
    {
        c[4]--,c[2]--,tot++;
        cards_type = "four_pair";
    }
    while(c[3]&&c[1])
    {
        c[3]--,c[1]--,tot++;
        cards_type = "three_one";
    }
    while(c[3]&&c[2])
    {
        c[3]--,c[2]--,tot++;
        cards_type = "three_pair";
    }
    if(c[1]) cards_type = "one";
    if(c[2])
    {
        cards_type = "pair";
        if(num[16]==2) cards_type="bomb";
    }
    if(c[3]) cards_type = "three_zero";
    if(c[4]) cards_type = "bomb";
    return tot+c[1]+c[2]+c[3]+c[4];
}
bool cards_check::chk_type()
{
    int now=go();
    if(now==1) return 1;
    for(int i=3;i<=13;++i) //三顺子
    {
        if(num[i]<3) continue;
        num[i]-=3;
        int sum=1;
        for(int j=1;j+i<=14;++j)
        {
            if(num[j+i]>=3) sum++;
            else break;
        }
        if(sum>=2)
        {
            for(int j=1;j<sum;++j)
                num[i+j]-=3;

            memset(c,0,sizeof(c));
            for(int i=3;i<=16;++i) c[num[i]]++;
            if(c[1]+c[2]+c[3]+c[4]==0)
            {
                cards_type = "three_zero_" + to_string(sum);
                return 0;
            }
            if(c[1]<=sum && c[2]+c[3]+c[4]==0) //c[1]:number of single card
            {
                cards_type = "three_one_" + to_string(sum);
                return 1;
            }
            if(c[2]<=sum && c[1]+c[3]+c[4]==0)
            {
                cards_type = "three_pair_" + to_string(sum);
                return 1;
            }

            for(int j=1;j<sum;++j) num[i+j]+=3;
        }
        num[i]+=3;
    }

    for(int i=3;i<=12;++i) //doublekill
    {
        if(num[i]<2) continue;
        num[i]-=2;
        int sum=1;
        for(int j=1;j+i<=14;++j)
        {
            if(num[j+i]>=2) sum++;
            else break;
        }
        if(sum>=3)
        {
            num[i+1]-=2;
            for(int j=2;j<sum;++j)
                num[i+j]-=2;

            memset(c,0,sizeof(c));
            for(int i=3;i<=16;++i) c[num[i]]++;
            if(c[1]+c[2]+c[3]+c[4]==0) //c[1]:number of single card
            {
                cards_type = "shifted_pair_" + to_string(sum);
                return 1;
            }

            for(int j=1;j<sum;++j) num[i+j]+=2;
        }
        num[i]+=2;
    }

    for(int i=3;i<=10;++i) //单顺子
    {
        if(!num[i]) continue;
        num[i]--;
        int sum=1;
        for(int j=1;j+i<=14;++j)
        {
            if(num[j+i]) sum++;
            else break;
        }
        if(sum>=5)
        {
            for(int j=1;j<=3;++j) num[i+j]--;
            for(int j=4;j<sum;++j)
                num[i+j]--;

            memset(c,0,sizeof(c));
            for(int i=3;i<=16;++i) c[num[i]]++;
            if(c[1]+c[2]+c[3]+c[4]==0) //c[1]:number of single card
            {
                cards_type = "sequence_" + to_string(sum);
                return 1;
            }
            for(int j=1;j<sum;++j) num[i+j]++;
        }
        num[i]++;
    }
    return 0;
}
string cards_check::number_to_info(int x)
{
    if(3<=x && x<=10) return to_string(x);
    if(x==11 ) return "J";
    if(x==12) return "Q";
    if(x==13) return "K";
    if(x==14) return "A";
    if(x==15) return "2";
    return "";
}

void cards_check::init_num()
{
    memset(num,0,sizeof(num));
    for(int i=0,sz=cds.size();i<sz;++i)
    {
        Cards cd=cds[i];
        int x;
        if('3'<=cd.info[0] && cd.info[0]<='9')
            x=stoi(cd.info);
        if(cd.info == "10")
            x=10;
        if(cd.info=="A")
            x=14;
        if(cd.info == "2")
            x=15;
        if(cd.info == "J")
            x=11;
        if(cd.info == "Q")
            x=12;
        if(cd.info == "K")
            x=13;
        if(cd.info=="BLACKJOKER" || cd.info == "REDJOKER")
            x=16;
        num[x]++;
    }
}
void cards_check::clear()
{
    cds.clear();
    memset(num,0,sizeof(num));
    memset(c,0,sizeof(c));
    cards_type.clear();
    cards_sequence.clear();
    val.clear();
}

int cards_check::chk(vector<Cards>&cds_)
{
    if(cds_.empty()) return 0;
    clear();
    cds = cds_;
    //push_("S7 C7 S5 H5 D5 H4 D4 C4 C3 S3");
//    push_("S3 S4 S5 S6 S7 C8 H9");;
    //push_("S3  C4 D4 H5 D5 D3");
    //push_("S3 D3 C3 S4 S5 C4 C5 H3");
//	push_("C4 D4 H4 S3 D3 H3 C5 C8");
    init_num();
    int rest = chk_type();
    if(!rest) return 0;
    init_num();

    if(cards_type.substr(0,4) == "four")
    {
        for(int i=3;i<=16;++i)
        {
            if(num[i] == 4)
            {
                Cards cd = "C"+number_to_info(i);
                for(Cards j : cds)
                {
                    if(j.info == cd.info)
                        cards_sequence.push_back(j);
                }
                val.push_back(cd);
            }
        }
        if(cards_type == "four_two" || cards_type == "four_one")
        {
            for(int i=3;i<=15;++i)
            {
                if(num[i] == 1)
                {
                    Cards cd = "C"+number_to_info(i);
                    for(Cards j : cds)
                    {
                        if(j.info == cd.info)
                            cards_sequence.push_back(j);
                    }
                }
            }
            if(num[16])
            {
                for(Cards j :cds)
                    if(j.info =="BLACKJOKER" || j.info == "REDJOKER")
                        cards_sequence.push_back(j);
            }
        }
        if(cards_type == "four_two_pair" || cards_type=="four_pair")
        {
            for(int i=3;i<=15;++i)
            {
                if(num[i] == 2)
                {
                    Cards cd = "C"+number_to_info(i);
                    for(Cards j : cds)
                    {
                        if(j.info == cd.info)
                            cards_sequence.push_back(j);
                    }
                }
            }
            if(num[16] == 2)
            {
                for(Cards j :cds)
                    if(j.info =="BLACKJOKER" || j.info == "REDJOKER")
                        cards_sequence.push_back(j);
            }
        }
    }
    //////****************
    if(cards_type.substr(0,5) == "three")
    {
        for(int i=3;i<=16;++i)
        {
            if(num[i] >= 3)
            {
                Cards cd = "C"+number_to_info(i);
                int tot_=0;
                for(Cards j : cds)
                {
                    if(j.info == cd.info && tot_+1<=3)
                        //cout<<"asd "<<j.cardid<<endl,
                        cards_sequence.push_back(j),tot_++;
                }
                val.push_back(cd);
                num[i]-=3;
            }
        }
        if(cards_type.substr(0,9) == "three_one") //including three_one_2
        {
            for(int i=3;i<=15;++i)
            {
                if(num[i] == 1)
                {
                    Cards cd = "C"+number_to_info(i);
                    for(Cards j : cds)
                    {
                        if(j.info == cd.info)
                        {
                            cards_sequence.push_back(j);
                            break;
                        }
                    }
                }
            }
            if(num[16])
            {
                for(Cards j :cds)
                    if(j.info =="BLACKJOKER" || j.info == "REDJOKER")
                        cards_sequence.push_back(j);
            }
        }
        if(cards_type.substr(0,10)=="three_pair")
        {
            for(int i=3;i<=15;++i)
            {
                if(num[i] == 2)
                {
                    Cards cd = "C"+number_to_info(i);
                    for(Cards j : cds)
                    {
                        if(j.info == cd.info)
                            cards_sequence.push_back(j);
                    }
                }
            }
            if(num[16] == 2)
            {
                for(Cards j :cds)
                    if(j.info =="BLACKJOKER" || j.info == "REDJOKER")
                        cards_sequence.push_back(j);
            }
        }
        if(cards_type.substr(0,10) == "three_zero")
        {
            //ok
        }
    }
    //////****************
    if(cards_type.substr(0,12) == "shifted_pair" || cards_type == "pair")
    {
        for(int i=3;i<=15;++i)
        {
            if(num[i] == 2)
            {
                Cards cd = "C"+number_to_info(i);
                for(Cards j : cds)
                {
                    if(j.info == cd.info)
                        cards_sequence.push_back(j);
                }
                val.push_back(cd);
            }
        }
    }
    if(cards_type.substr(0,8) == "sequence" || cards_type =="one")
    {
        for(int i=3;i<=15;++i)
        {
            if(num[i] == 1)
            {
                Cards cd = "C"+number_to_info(i);
                for(Cards j : cds)
                {
                    if(j.info == cd.info)
                        cards_sequence.push_back(j);
                }
                val.push_back(cd);
            }
        }
        if(num[16]==1)
        {
            for(Cards j :cds)
                if(j.info =="BLACKJOKER" || j.info == "REDJOKER")
                {
                    cards_sequence.push_back(j);
                    val.push_back(j);
                }
        }
    }
    if(cards_type == "bomb")
    {
        if(num[16]==2)
        {
            cards_sequence.push_back("BLACKJOKER");
            cards_sequence.push_back("REDJOKER");
            val.push_back("REDJOKER");
        }
        for(int i=3;i<=15;++i)
        {
            if(num[i]==4)
            {
                Cards cd = "C"+number_to_info(i);
                for(Cards j : cds)
                {
                    if(j.info == cd.info)
                        cards_sequence.push_back(j);
                }
                val.push_back(cd);
            }
        }
    }

    return rest;
}
