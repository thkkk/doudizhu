#include "mythread.h"

mythread::mythread()
{

}

void mythread::run()
{
    MainWindow server;

    //while(1) { QThread::msleep(1);}
    exec();
}
