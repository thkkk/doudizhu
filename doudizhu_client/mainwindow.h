#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtNetwork>
#include<QString>
#include<string>
#include<sstream>
#include<QObject>
using std::string;
using std::to_string;
using std::stringstream;


class MainWindow :QObject
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();
    int Rand(int l,int r);

    void initserver();
    void initcards();
    void dealcards();
    void WantLandlord();
    void DecideLandlord();
    void Startgame();
    void Prepgame();

    void clear();

public slots:
    void acceptConnection();
    void recv_0();
    void recv_1();
    void recv_2();
    void recvinfo(int ID);
    void sendinfo(QString s, int player=3);

private:
    QTcpServer* listenSocket;
    QTcpSocket* readwriteSocket[3];
    int ConnectionNumber , Restartnumber;
    bool BeLandlord[3];
    string cards[54];
    int Landlords;
    bool End;

    int first_player_called;
};

#endif // MAINWINDOW_H
