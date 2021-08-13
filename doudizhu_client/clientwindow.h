#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include<QtNetwork>
#include<QString>
#include<sstream>
#include<string>
#include"cards.h"
#include<vector>
#include <QRect>
#include <QPainter>
#include<QMouseEvent>
#include<QFrame>
#include<QBitmap>
#include<QTextBrowser>
#include"cards_check.h"
#include"played_cards.h"

using std::stringstream;
using std::string;
using std::vector;

namespace Ui {
class clientWindow;
}

class clientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit clientWindow(QWidget *parent = nullptr);
    ~clientWindow();

    void sendinfo(QString s);
    void identity_display();
    void call_display(QString s, int x);
    void Change_cnt_text(QTextBrowser* qtb,int x);
    void Change_infotext(QString s);
    void set_Idtext();

    void display_cards();
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent* ev);
    void resizeEvent(QResizeEvent *ev);

    void clear();

public slots:
    void recvinfo();
    void connectHost();
    void Yesfunc();
    void Nofunc();
    void Restart();

private:
    Ui::clientWindow *ui;
    QTcpSocket* readwriteSocket;
    int id;
    string state;
    vector<Cards> cards;
    vector<Cards>landlord_cards;
    int landlord;
    int cards_cnt[3];
    cards_check chktool;

    Played_cards played_cards[3];
    bool Buchu[3];
    int last_played_player;
    int nowplayer;

};

#endif // CLIENTWINDOW_H
