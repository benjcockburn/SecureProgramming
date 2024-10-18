//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)
#include <string>

#include <thread>
#include <mutex>

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>   // For inet_addr and htons
#include <unistd.h>
#include <QObject>
#include "JsonHandler.h"

#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H





class messageHandler: public QObject {
    Q_OBJECT


public:
    messageHandler(QObject *parent = nullptr);
    bool sendMessage(const char* message,int port);
    // int connectAndSend(const char* server_ip, int port, const char* message);
    volatile bool thread_receiving=false;
    volatile bool thread_sending= false;

    bool start();


    int server();

    ~messageHandler();


signals:
    void messageReceived(QString message, QString recipient, QString sender);

private:
    int result = -1;
};

#endif // MESSAGEHANDLER_H
