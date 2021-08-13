#ifndef MYTHREAD_H
#define MYTHREAD_H
#include<QThread>
#include<QMediaPlayer>
#include<QMediaPlaylist>
#include"mainwindow.h"

class mythread : public QThread
{
public:
    mythread();
protected:
    virtual void run();
};

#endif // MYTHREAD_H
