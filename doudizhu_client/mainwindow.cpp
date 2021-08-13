#include "mainwindow.h"
#include <chrono>

MainWindow::MainWindow(QWidget *parent):
    QObject(parent)
{
    ConnectionNumber=0; Restartnumber=0;
    BeLandlord[0]=BeLandlord[1]=BeLandlord[2]=0;
    Landlords = -1;
    End=0;
    first_player_called = -1;
    initserver();

    qDebug() << "Current thread:" << thread();

}

MainWindow::~MainWindow()
{
}
int MainWindow::Rand(int l, int r)
{
    return rand()%(r-l+1)+l;
}

void MainWindow::initserver()
{
    listenSocket = new QTcpServer;
    listenSocket->listen(QHostAddress("183.172.205.3"),8888);
    QObject::connect(listenSocket , SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

void MainWindow::acceptConnection()
{
    if(ConnectionNumber ==3) return ;
    readwriteSocket[ConnectionNumber] = listenSocket->nextPendingConnection();
    //auto func = (ConnectionNumber==0)? recv_0 :(ConnectionNumber == 1? recv_1:recv_2);
    //if(ConnectionNumber==0) QObject::connect(readwriteSocket[ConnectionNumber], SIGNAL(readyRead()) , this, SLOT(recv_0()) );
    //if(ConnectionNumber==1) QObject::connect(readwriteSocket[ConnectionNumber], SIGNAL(readyRead()) , this, SLOT(recv_1()) );
    //if(ConnectionNumber==2) QObject::connect(readwriteSocket[ConnectionNumber], SIGNAL(readyRead()) , this, SLOT(recv_2()) );

    ConnectionNumber++;
    std::this_thread::sleep_for(std::chrono:: milliseconds (500));
    if(ConnectionNumber == 3)
    {
        QObject::connect(readwriteSocket[0], SIGNAL(readyRead()) , this, SLOT(recv_0()) );
        QObject::connect(readwriteSocket[1], SIGNAL(readyRead()) , this, SLOT(recv_1()) );
        QObject::connect(readwriteSocket[2], SIGNAL(readyRead()) , this, SLOT(recv_2()) );
        Prepgame();
    }
}
void MainWindow::recv_0(){ recvinfo(0);}
void MainWindow::recv_1(){ recvinfo(1);}
void MainWindow::recv_2(){ recvinfo(2);}
void MainWindow::recvinfo(int ID)
{
    //for(int i=0;i<ConnectionNumber;++i)
    //{
        int i = ID;
        QString info_= readwriteSocket[i]->readAll();
        qDebug()<<info_;
        if(info_ == "") return ;
        stringstream ss;
        ss<<info_.toStdString();
        string  info;
        ss >> info;
        if(info == "叫")
        {
            BeLandlord[i] = 1;
            sendinfo(info_, i);
            std::this_thread::sleep_for(std::chrono:: milliseconds (500));
            if((i+1)%3 != first_player_called) sendinfo("叫地主",(i+1)%3);
            else DecideLandlord();
        }
        else if(info =="不叫")
        {
            BeLandlord[i]=0;
            sendinfo(info_, i);
            std::this_thread::sleep_for(std::chrono:: milliseconds (500));
            if((i+1)%3 != first_player_called) sendinfo("叫地主",(i+1)%3);
            else DecideLandlord();
        }
        else if(info == "出牌")
        {
            sendinfo(info_, i);
            std::this_thread::sleep_for(std::chrono:: milliseconds (500));
            if(!End) sendinfo("turn",(i+1)%3);
        }
        else if(info == "不出")
        {
            sendinfo(info_,i);
            std::this_thread::sleep_for(std::chrono:: milliseconds (500));
            if(!End) sendinfo("turn",(i+1)%3);
        }
        else if(info == "结束")
        {
            End=1;
            string res ,ls;  ss >> res;
            QString last_cards= " ";
            while(ss >> ls) last_cards += QString::fromStdString(ls) + " ";
            sendinfo(last_cards,i);
            std::this_thread::sleep_for(std::chrono:: milliseconds (500));
            info_ = QString::fromStdString(info) +" " + QString::fromStdString(res)+ " ";
            sendinfo(info_,3);
        }
        else if(info == "重新开始")
        {
            Restartnumber++;
        }
    //}
    if(Restartnumber == 3)
    {
        clear();
        Prepgame();
    }
}
void MainWindow::clear()
{
    Restartnumber=0;
    BeLandlord[0]=BeLandlord[1]=BeLandlord[2]=0;
    Landlords = -1;
    End=0;
    first_player_called = -1;
}

void MainWindow::sendinfo(QString s,int player)
{
    if(s == "id ")
    {
        s = QString::number(player) + " " + s +"           ";
        qDebug()<<s<<'\n';
        QByteArray* array = new QByteArray;
        array->clear();
        array->append(s);
        readwriteSocket[player]->write(array->data());
        readwriteSocket[player]->flush();
        readwriteSocket[player]->waitForBytesWritten();
        return ;
    }
    if(player == -1) s = "              ";
    else s = QString::number(player) + " " + s +"         ";
    qDebug()<<s<<'\n';
    QByteArray* array = new QByteArray;
    array->clear();
    array->append(s);
    for(int i=0;i<ConnectionNumber;++i)
    {
        readwriteSocket[i]->write(array->data());
        readwriteSocket[i]->flush();
        readwriteSocket[i]->waitForBytesWritten();
    }
}

void MainWindow::initcards()
{
    string ls1="C",ls2="3";
    for(int i=0;i<54;++i)
    {
        if(ls2 == "88") cards[i]=ls1;
        else cards[i]=ls1+ls2;

        if(ls2 == "J")
            ls2 = "Q";
        else if(ls2=="Q")
            ls2 = "K";
        else if(ls2=="K")
            ls2 = "A";
        else if(ls2 == "A")
            ls2 = "2";
        else if(ls2 == "2")
        {
            if(ls1 == "C") ls1 ="D" , ls2="3";
            else if(ls1 == "D") ls1="H", ls2="3";
            else if(ls1=="H") ls1="S", ls2="3";
            else if(ls1=="S") ls1="BLACKJOKER" , ls2="88";
        }
        else if(ls1 == "BLACKJOKER")
            ls1="REDJOKER";
        else if(3<=stoi(ls2) && stoi(ls2) <= 9)
            ls2 = to_string(stoi(ls2) + 1);
        else if(stoi(ls2) == 10)
            ls2 = "J";
    }
    random_shuffle(cards,cards+54);
    //for(int i=0;i<54;++i)
    //    qDebug()<<QString::fromStdString(cards[i]);
}
void MainWindow::dealcards()
{
    initcards();
    QString s;
    s="dealcards ";
    for(int i=0;i<17;++i)
        s += QString::fromStdString(cards[i]) + " ";
    sendinfo(s,0);

    s="dealcards ";
    for(int i=17;i<34;++i)
        s +=QString::fromStdString(cards[i]) + " ";
    sendinfo(s,1);

    s="dealcards ";
    for(int i=34;i<51;++i)
        s +=QString::fromStdString(cards[i]) + " ";
    sendinfo(s,2);
}
void MainWindow::WantLandlord()
{
    sendinfo("state 抢地主");
    int p = rand()%3;
    first_player_called = p;
    sendinfo("叫地主",p);
}
void MainWindow::DecideLandlord()
{
    int p= first_player_called;
    if(BeLandlord[(p+2)%3])
    {
        Landlords = (p+2)%3 ;
        sendinfo("成为地主",(p+2)%3);

        QString s;
        s="dealcards3 ";
        for(int i=51;i<54;++i)
            s +=QString::fromStdString(cards[i]) + " ";
        sendinfo(s,(p+2)%3);
    }
    else if(BeLandlord[(p+1)%3])
    {
        Landlords = (p+1)%3 ;
        sendinfo("成为地主",(p+1)%3);

        QString s;
        s="dealcards3 ";
        for(int i=51;i<54;++i)
            s +=QString::fromStdString(cards[i]) + " ";
        sendinfo(s,(p+1)%3);
    }
    else {
        Landlords = p ;
        sendinfo("成为地主",p);

        QString s;
        s="dealcards3 ";
        for(int i=51;i<54;++i)
            s +=QString::fromStdString(cards[i]) + " ";
        sendinfo(s,p);
    }
    std::this_thread::sleep_for(std::chrono:: milliseconds (1000));
    Startgame();
}
void MainWindow::Prepgame()
{
    qDebug()<<"Start asd ";
    for(int i=0;i<3;++i)
    {
        QString s = "id ";
        sendinfo(s,i);
    }
    std::this_thread::sleep_for(std::chrono:: milliseconds (1000));
    qDebug()<<"Start dealcards ";
    dealcards();

    std::this_thread::sleep_for(std::chrono:: milliseconds (1000));
    qDebug()<<"Start WantLandlord ";
    WantLandlord();
}
void MainWindow::Startgame()
{


    std::this_thread::sleep_for(std::chrono:: milliseconds (1000));
    int nowplayer = Landlords;
    sendinfo("turn",nowplayer);
    /*for(;;nowplayer=(nowplayer+1)%3)
    {
        sendinfo("turn",nowplayer);
        if(readwriteSocket[nowplayer]->waitForReadyRead(300000))
        {
            qApp->processEvents();
        }
        Sleep(1000);
        //recvinfo();
        if(End) break;
    }*/
}
