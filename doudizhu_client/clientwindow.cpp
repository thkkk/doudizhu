#include "clientwindow.h"
#include "ui_clientwindow.h"

clientWindow::clientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::clientWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->connectButton, SIGNAL(clicked()), this , SLOT(connectHost()));
    QObject::connect(ui->YesButton, SIGNAL(clicked()) , this, SLOT(Yesfunc()));
    QObject::connect(ui->NoButton, SIGNAL(clicked()) , this, SLOT(Nofunc()));
    QObject::connect(ui->RestartButton, SIGNAL(clicked()) , this , SLOT(Restart()));
    QObject::connect(ui->QuitButton, SIGNAL(clicked()) , this , SLOT(close()));
    ui->YesButton->setEnabled(0);
    ui->NoButton->setEnabled(0);
    ui->RestartButton->setEnabled(0);
    ui->QuitButton->setEnabled(0);
    cards_cnt[0] = cards_cnt[1]=cards_cnt[2]=0;
    nowplayer = -1;
    last_played_player = -1;
    played_cards[0].clear();
    played_cards[1].clear();
    played_cards[2].clear();
    Buchu[0]= Buchu[1]=Buchu[2]=0;
    id=0;

    ////////////////////
    /*id=0;
    cards.push_back("C10");
    cards.push_back("C3");
    cards.push_back("C4");
    cards.push_back("C5");
    cards.push_back("C6");
    cards.push_back("C7");
    cards.push_back("C8");
    cards.push_back("C9");
    cards.push_back("C10");
    cards.push_back("C10");
    cards.push_back("C10");
    cards.push_back("C10");
    cards.push_back("C10");
    cards.push_back("C10");
    cards.push_back("C10");
    cards.push_back("C10");
    cards.push_back("C10");
    cards.push_back("C10");
    cards.push_back("C10");
    cards.push_back("C10");

    landlord_cards.push_back("DK");
    landlord_cards.push_back("DK");
    landlord_cards.push_back("DK");

    played_cards[0].cards_sequence.push_back("H5");
    played_cards[0].cards_sequence.push_back("H5");
    played_cards[0].cards_sequence.push_back("H5");
    played_cards[0].cards_sequence.push_back("H5");
    played_cards[0].cards_sequence.push_back("H5");
    played_cards[0].cards_sequence.push_back("H5");

    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");
    played_cards[1].cards_sequence.push_back("H5");

    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    played_cards[2].cards_sequence.push_back("D5");
    nowplayer = 2;

    cards_cnt[0] = cards_cnt[1]=cards_cnt[2]=17;*/
}

clientWindow::~clientWindow()
{
    delete ui;
}

void clientWindow::clear()
{
    ui->YesButton->setEnabled(0);
    ui->NoButton->setEnabled(0);
    ui->RestartButton->setEnabled(0);
    ui->QuitButton->setEnabled(0);
    id=0; state = "";
    cards.clear();  landlord_cards.clear();
    landlord = 0;
    cards_cnt[0] = cards_cnt[1]=cards_cnt[2]=0;
    chktool.clear();
    nowplayer = -1;
    last_played_player = -1;
    played_cards[0].clear();
    played_cards[1].clear();
    played_cards[2].clear();
    Buchu[0]= Buchu[1]=Buchu[2]=0;
    ui->textBrowser_0->setText("");
    ui->textBrowser_1->setText("");
    ui->textBrowser_2->setText("");
}
void clientWindow::Restart()
{
    clear();
    Change_infotext("请等待");
    update();
    sendinfo("重新开始");
}

void clientWindow::connectHost()
{
    Change_infotext("请等待");
    readwriteSocket = new QTcpSocket;
    readwriteSocket->connectToHost(QHostAddress("183.172.205.3"),8888);
    QObject::connect(readwriteSocket , SIGNAL(readyRead()) , this , SLOT(recvinfo()));
    ui->connectButton->setEnabled(0);
}

void clientWindow::sendinfo(QString s)
{
    s += " ";
    QByteArray* array = new QByteArray;
    array->clear();
    array->append(s);
    readwriteSocket->write(array->data());
    readwriteSocket->flush();
    readwriteSocket->waitForBytesWritten();
}


void clientWindow::recvinfo()
{
    QString info = readwriteSocket->readAll();
    qDebug()<<info<<endl;
    stringstream ss;
    ss << info.toStdString();
    int player=-1;
    string typ;

    while(ss>>player)
    {
    //while begin
    ss>>typ;
    if(typ == "id")
    {
        id=player;
        continue;
    }
    if(typ=="state") set_Idtext();
    if(player == id ||player==3)
    {
        if(typ=="state")
        {
            ss>>typ;
            if(typ =="抢地主")
            {
                state = typ;
                ui->YesButton->setText("叫地主");
                ui->NoButton->setText("不叫");
            }
        }
        else if(typ=="dealcards")
        {
            string card;
            for(int i=0;i<17;++i)
            {
                ss >>card;
                cards.push_back(card);
            }
            sort(cards.begin(),cards.end());
            reverse(cards.begin(),cards.end());
            //qDebug()<<cards[0].cardid.c_str()<<" asd ";
            update();
        }
        else if(typ=="dealcards3")
        {
            string card;
            for(int i=0;i<3;++i)
            {
                ss >>card;
                cards.push_back(card);
                landlord_cards.push_back(card);
            }
            sort(cards.begin(),cards.end());
            reverse(cards.begin(),cards.end());
            update();
        }
        else if(typ == "叫地主")
        {
            Change_infotext("请选择是否叫地主");
            ui->YesButton->setEnabled(1);
            ui->NoButton->setEnabled(1);
        }
        else if(typ == "叫" || typ == "不叫")
        {
            call_display(QString::fromStdString(typ == "叫"? "叫地主":"不叫") , 0);
        }
        else if(typ=="成为地主")
        {
            Change_infotext("你成为地主");
            landlord = id;
            identity_display();
            ui->YesButton->setText("出牌");
            ui->NoButton->setText("不出");
            ui->YesButton->setEnabled(0);
            ui->NoButton->setEnabled(0);
            state = "出牌阶段";
            last_played_player=-1;
        }
        else if(typ == "turn")
        {
            Buchu[player]=0;
            nowplayer = player;
            Change_infotext("请出牌");
            played_cards[id].clear();
            ui->YesButton->setEnabled(1);
            ui->NoButton->setEnabled(1);
            update();
        }
        else if(typ=="出牌")
        {
            last_played_player = player;
            string s;
            ss >> s;
            int num;
            ss >> num;
            for(int i=0;i<num;++i)
                ss>>s;
            int valnum;
            ss>>valnum;
            for(int i=0;i<valnum;++i)
                ss>>s;
            update();
        }
        else if(typ == "不出")
        {
            played_cards[player].clear();
            Buchu[player]=1;
            update();
        }
        else if(typ == "结束")
        {
            ss >> typ;
            if(typ == "地主获胜")
            {
                if(id == landlord) Change_infotext(QString::fromStdString(typ) + "!" + "你赢了!继续请点重新开始");
                if(id != landlord) Change_infotext(QString::fromStdString(typ) + "!" + "你输了!继续请点重新开始");
            }
            else if(typ == "农民获胜")
            {
                if(id != landlord) Change_infotext(QString::fromStdString(typ) + "!" + "你赢了!继续请点重新开始");
                if(id == landlord) Change_infotext(QString::fromStdString(typ) + "!" + "你输了!继续请点重新开始");
            }
            ui->YesButton->setEnabled(0);
            ui->NoButton->setEnabled(0);
            ui->RestartButton->setEnabled(1);
            ui->QuitButton->setEnabled(1);
        }
    }
    else //*********** other player *************
    {
        if(typ=="state")
        {
            ss>>typ;
            if(typ =="抢地主")
            {
                //ok
            }
        }
        else if(typ=="dealcards")
        {
            string card;
            for(int i=0;i<17;++i)
            {
                ss >>card;
                cards_cnt[player]++;
            }//
            update();
        }
        else if(typ=="dealcards3")
        {
            string card;
            for(int i=0;i<3;++i)
            {
                ss >>card;
                landlord_cards.push_back(card);
                cards_cnt[player]++;
            }
            update();
        }
        else if(typ == "叫地主")
        {
            Change_infotext(QString::number(player)+"号玩家正在决定是否叫地主");
        }
        else if(typ == "叫" || typ == "不叫")
        {
            call_display(QString::fromStdString(typ == "叫"? "叫地主":"不叫") , (player-id+3)%3);
        }
        else if(typ=="成为地主")
        {
            Change_infotext(QString::number(player)+"号玩家成为地主");
            landlord = player;
            identity_display();
            ui->YesButton->setText("出牌");
            ui->NoButton->setText("不出");
            state = "出牌阶段";
            last_played_player=-1;
        }
        else if(typ=="turn")
        {
            played_cards[player].clear();
            Buchu[player]=0;
            nowplayer = player;
            Change_infotext(QString::number(player)+"号玩家正在出牌");
            update();
        }
        else if(typ=="出牌")
        {
            last_played_player = player;
            played_cards[player].clear();
            ss >> played_cards[player].cards_type;
            int num;
            ss >> num;
            string s;
            for(int i=0;i<num;++i)
            {
                ss>>s;
                played_cards[player].cards_sequence.push_back(s);
                cards_cnt[player] --;
            }
            int valnum;
            ss>>valnum;
            for(int i=0;i<valnum;++i)
            {
                ss>>s;
                played_cards[player].val.push_back(s);
            }
            update();
        }
        else if(typ == "不出")
        {
            played_cards[player].clear();
            Buchu[player]=1;
            update();
        }
    }
    //while end
    }
}
void clientWindow::call_display(QString s,int x)
{
    s=  "<center><b><font size=5 color=\"#ffff00\">"+s+"</font></b></center>";
    if(x==0)
        ui->textBrowser_0->setText(s);
    else if(x==1)
        ui->textBrowser_1->setText(s);
    else if(x==2)
        ui->textBrowser_2->setText(s);
}
void clientWindow::identity_display()
{
    /*QFont ft; ft.setPointSize(16);
    ui->textBrowser_0->setTextColor((landlord==id) ?QColor(255,255,0):QColor(255,170,0));
    ui->textBrowser_0->setText( (landlord==id) ?"地主":"农民");

    ui->textBrowser_1->setTextColor((landlord==(id+1)%3) ?QColor(255,255,0):QColor(255,170,0));
    ui->textBrowser_1->setText( (landlord==(id+1)%3) ?"地主":"农民");

    ui->textBrowser_2->setTextColor((landlord==(id+2)%3) ?QColor(255,255,0):QColor(255,170,0));
    ui->textBrowser_2->setText( (landlord==(id+2)%3) ?"地主":"农民");*/

    QString dizhu = "<center><b><font size=5 color=\"#ffff00\">地主</font></b></center>";
    QString nongmin = "<center><b><font size=5 color=\"#ffaa00\">农民</font></b></center>";
    ui->textBrowser_0->setText((landlord==id) ?dizhu:nongmin);
    ui->textBrowser_1->setText( (landlord==(id+1)%3) ?dizhu:nongmin);
    ui->textBrowser_2->setText( (landlord==(id+2)%3) ?dizhu:nongmin);
}
void clientWindow::set_Idtext()
{
    ui->Idtext_0->setText("<center><b><font size=5 color=\"#ffffff\">"+QString::number(id)+"号玩家"+"</font></b></center>");
    ui->Idtext_1->setText("<center><b><font size=5 color=\"#ffffff\">"+QString::number((id+1)%3)+"号玩家"+"</font></b></center>");
    ui->Idtext_2->setText("<center><b><font size=5 color=\"#ffffff\">"+QString::number((id+2)%3)+"号玩家"+"</font></b></center>");
}
void clientWindow::Yesfunc()
{
    if(state == "抢地主")
    {
        sendinfo("叫");
        ui->YesButton->setEnabled(0);
        ui->NoButton->setEnabled(0);
    }
    else if(state == "出牌阶段")
    {
        vector<Cards>cds;
        cds.clear();
        for(int i=0;i<cards.size();++i) if(cards[i].lift)
            cds.push_back(cards[i]);

        int res = chktool.chk(cds);
        played_cards[id].cards_type = chktool.cards_type;
        played_cards[id].cards_sequence = chktool.cards_sequence;
        played_cards[id].val = chktool.val;
        if(last_played_player != -1 && last_played_player!=id)
        {
            res &= played_cards[id].greater_comp(played_cards[last_played_player]);
        }
        if(res)
        {
            Change_infotext("出牌合法");
            cds.clear();
            for(int i=0;i<cards.size();++i) if(!cards[i].lift)
                cds.push_back(cards[i]);
            cards  = cds;
            QString s = "出牌 " + QString::fromStdString(chktool.cards_type) + " ";

            s+=QString::number(chktool.cards_sequence.size()) + " ";
            for(auto x : chktool.cards_sequence)
                s += QString::fromStdString(x.cardid) + " ";

            s+=QString::number(chktool.val.size()) + " ";
            for(auto x : chktool.val)
                s+= QString::fromStdString(x.cardid) + " ";
            update();
            ui->YesButton->setEnabled(0);
            ui->NoButton->setEnabled(0);
            last_played_player = id;
            if(!cards.size())
            {
                QString qs;
                if(id==landlord) qs = "结束 地主获胜  ";
                else qs="结束 农民获胜  ";
                qDebug()<<qs;
                sendinfo(qs+s);
            }
            else sendinfo(s);
        }
        else
        {
            Change_infotext("出牌不合法");
            played_cards[id].clear();
        }
    }
}
void clientWindow::Nofunc()
{
    if(state == "抢地主")
    {
        sendinfo("不叫");
        ui->YesButton->setEnabled(0);
        ui->NoButton->setEnabled(0);
    }
    else if(state == "出牌阶段")
    {
        if(last_played_player == -1 || last_played_player==id)
        {
            Change_infotext("请出牌！");
            return ;
        }
        for(int sz=cards.size(),i=0;i<sz;++i)
            cards[i].lift = 0;
        ui->YesButton->setEnabled(0);
        ui->NoButton->setEnabled(0);
        update();
        sendinfo("不出");
    }
}
void clientWindow::Change_cnt_text(QTextBrowser* qtb,int x)
{
    /*QFont ft; ft.setPointSize(16);
    //qtb->set
    qtb->setFont(ft);
    qtb->setTextColor(QColor(170,255,127));
    qtb->setText(QString::number(cards_cnt[(id+x)%3]));*/

    qtb->setText("<center><font size=6 color=\"#aaff7f\">"+QString::number(cards_cnt[(id+x)%3])+"</font></center>");
}
void clientWindow::Change_infotext(QString s)
{
    ui->InfotextBrowser->setText("<center><b><font size=7 color=\"#ff5500\">"+s+"</font></b></center>");
}
void clientWindow::resizeEvent(QResizeEvent *ev)
{
    update();
}
void clientWindow::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    int w = this->frameGeometry().width(), h =this->frameGeometry().height();

    //p.drawPixmap(QRect(100,500,91,126),QPixmap(":/new/prefix1/pics/C3.png"));
    //p.drawPixmap(QRect(120,500,91,126),QPixmap(":/new/prefix1/pics/C2.png"));
    //Button:
    ui->cnt_text_1->setGeometry(1040.0/1142*w, 80.0/917 * h, 101.0/1142*w, 51.0/917 * h);
    ui->cnt_text_2->setGeometry(20.0/1142*w , 80.0/917 * h , 101.0/1142*w, 51.0/917 * h);
    ui->Idtext_0->setGeometry(580.0/1142*w, 820.0/917 * h, 111.0/1142*w ,41.0/917 * h);
    ui->Idtext_1->setGeometry(1030.0/1142*w, 140.0/917 * h, 111.0/1142*w, 41.0/917 * h);
    ui->Idtext_2->setGeometry(20.0/1142*w, 140.0/917 * h , 111.0/1142*w  , 41.0/917 * h);
    ui->textBrowser_0->setGeometry(460.0/1142*w, 820.0/917 * h, 121.0/1142*w, 41.0/917 * h);
    ui->textBrowser_1->setGeometry(920.0/1142*w, 140.0/917 * h, 111.0/1142*w, 41.0/917 * h);
    ui->textBrowser_2->setGeometry(130.0/1142*w, 140.0/917 * h, 111.0/1142*w, 41.0/917 * h);
    ui->InfotextBrowser->setGeometry(380.0/1142*w , 150.0/917 * h, 371.0/1142*w, 111.0/917 * h);
    ui->YesButton->setGeometry(440.0/1142*w, 570.0/917 * h, 131.0/1142*w, 32.0/917 * h);
    ui->NoButton->setGeometry(580.0/1142*w, 570.0/917 * h, 131.0/1142*w, 32.0/917 * h);
    ui->connectButton->setGeometry(1020.0/1142*w, 810.0/917 * h, 121.0/1142*w, 41.0/917 * h);
    ui->RestartButton->setGeometry(0.0/1142*w,810.0/917 * h, 121.0/1142*w, 41.0/917 * h);
    ui->QuitButton->setGeometry(120.0/1142*w, 810.0/917 * h, 121.0/1142*w, 41.0/917 * h);

    // .0/1142*w  .0/917 * h
    int num=cards.size();
    int intv = 40.0/1142*w;
    int xst = 560.0/1142*w - num/2*intv;

    string path_=":/new/prefix1/pics/";
    for(int i=0;i<num;++i)
    {
        int y = cards[i].y;
        if(!y) y=620.0 / 917 * h;
        if(cards[i].lift) y= 620.0 / 917 * h-10;
        else y = 620.0 / 917 * h;
        p.drawPixmap(QRect(xst,y,117.0/1142*w,162.0/917 * h),QPixmap( (path_ + cards[i].cardid +".png" ).c_str() ) );
        cards[i].set(xst,y,117.0/1142*w,162.0/917 * h);
        xst+=intv;
    }

    intv = 79.0/1142*w;
    xst = 560.0/1142*w- intv*1.5;
    if(state == "抢地主")
    {
        for(int i=0;i<3;++i)
        {
            p.drawPixmap(QRect(xst,14.0 / 917 * h,78.0/1142*w,108.0 / 917 * h),QPixmap( (path_ + "PADDING.png" ).c_str() ) );
            xst+=83.0/1142*w;
        }
    }
    else
    {
        for(int sz=landlord_cards.size(),i=0;i<sz;++i)
        {
            p.drawPixmap(QRect(xst,14.0 / 917 * h,78.0/1142*w,108.0 / 917 * h),QPixmap( (path_ + landlord_cards[i].cardid +".png" ).c_str() ) );
            xst+=83.0/1142*w;
        }
    }


    //other player
    //cards_cnt[0] = cards_cnt[1]=cards_cnt[2]=17;////////
    Change_cnt_text(ui->cnt_text_1,1);
    Change_cnt_text(ui->cnt_text_2,2);

    if(cards_cnt[(id+1)%3])
    {
        int yst=190.0/917 * h;
        for(int i=0;i<cards_cnt[(id+1)%3];++i)
        {
            p.drawPixmap(QRect(1029.0/1142*w,yst,78.0/1142*w,108.0 / 917 * h),QPixmap( (path_ + "PADDING.png" ).c_str() ) );
            yst+=15.0/917 * h;
        }
    }
    if(cards_cnt[(id+2)%3])
    {
        int yst=190.0/917 * h;
        for(int i=0;i<cards_cnt[(id+2)%3];++i)
        {
            p.drawPixmap(QRect(40.0/1142*w,yst,78.0/1142*w,108.0 / 917 * h),QPixmap( (path_ + "PADDING.png" ).c_str() ) );
            yst+=15.0/917 * h;
        }
    }

    if(!played_cards[id].empty())
    {
        intv = 30.0/1142*w;
        num = played_cards[id].cards_sequence.size();
        xst = 560.0/1142*w - num/2*intv;
        for(int i=0;i<num;++i)
        {
            Cards cd= played_cards[id].cards_sequence[i];
            p.drawPixmap(QRect(xst,440.0 / 917 * h,78.0/1142*w,108.0 / 917 * h),QPixmap( (path_ + cd.cardid +".png" ).c_str() ) );
            xst+=intv;
        }
    }
    else {
        if(Buchu[id])
        {
            xst = 560.0/1142*w - 25.0/1142*w;
            p.drawPixmap(QRect(xst,480.0 / 917 * h,97.0/1142*w,50.0 / 917 * h),QPixmap( (path_ + "buchu.png" ).c_str() ) );
        }
    }

    if(!played_cards[(id+1)%3].empty())
    {
        intv = 20.0/1142*w;
        num = played_cards[(id+1)%3].cards_sequence.size();
        xst = 1040.0/1142*w - num*intv - 90.0/1142*w;
        for(int i=0;i<num;++i)
        {
            Cards cd= played_cards[(id+1)%3].cards_sequence[i];
            p.drawPixmap(QRect(xst,250.0 / 917 * h,78.0/1142*w,108.0 / 917 * h),QPixmap( (path_ + cd.cardid +".png" ).c_str() ) );
            xst+=intv;
        }
    }
    else {
        //qDebug()<<ui->InfotextBrowser->toPlainText()<<"asdasdasd ";
        if(Buchu[(id+1)%3])
        {
            xst = 1000.0/1142*w - 100.0/1142*w;
            p.drawPixmap(QRect(xst,250.0 / 917 * h,97.0/1142*w,50.0 / 917 * h),QPixmap( (path_ + "buchu.png" ).c_str() ) );
        }
    }

    if(!played_cards[(id+2)%3].empty())
    {
        intv = 20.0/1142*w;
        num = played_cards[(id+2)%3].cards_sequence.size();
        xst = 159.0/1142*w;
        for(int i=0;i<num;++i)
        {
            Cards cd= played_cards[(id+2)%3].cards_sequence[i];
            p.drawPixmap(QRect(xst,250.0 / 917 * h,78.0/1142*w,108.0 / 917 * h),QPixmap( (path_ + cd.cardid +".png" ).c_str() ) );
            xst+=intv;
        }
    }
    else {
        if(Buchu[(id+2)%3])
        {
            xst = 159.0/1142*w ;
            p.drawPixmap(QRect(xst,250.0 / 917 * h,97.0/1142*w,50.0 / 917 * h),QPixmap( (path_ + "buchu.png" ).c_str() ) );
        }
    }
}

void clientWindow::mousePressEvent(QMouseEvent* ev)
{
    if(!cards.size()) return ;
    int num=cards.size();
    QPoint qp=ev->pos();

    bool changed=0;
    for(int i=num-1;i>=0;--i)
    {
        if(cards[i].y<=qp.y() && qp.y()<=cards[i].y+cards[i].h &&
                cards[i].x<=qp.x() && qp.x()<=cards[i].x+cards[i].w)
        {
            changed=1;
            cards[i].lift ^=1;
            if(cards[i].lift == 1) cards[i].y=620 - 10;
            else  cards[i].y=620;
            break;
        }
    }
    if(changed) update();
}

