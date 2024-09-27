
#include <string>

#include <thread>
#include <mutex>

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>
#include <QObject>


#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H





class messageHandler: public QObject {
    Q_OBJECT

public:
    messageHandler(QObject *parent = nullptr);
    bool sendMessage();

    volatile bool thread_receiving=false;
    volatile bool thread_sending= false;

    void start();


    int server();

    ~messageHandler();


signals:
    void messageReceived(QString message, QString recipient, QString sender);


};

#endif // MESSAGEHANDLER_H
