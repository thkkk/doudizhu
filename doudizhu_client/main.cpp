#include "clientwindow.h"
#include <QApplication>
#include"mythread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mythread* th = new mythread;
    th->start();

    /*clientWindow w[3];
    w[0].show();
    w[1].show();
    w[2].show();*/
    clientWindow w;
    w.show();
    return a.exec();
}
