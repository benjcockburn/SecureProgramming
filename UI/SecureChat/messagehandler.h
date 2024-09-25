#include <iostream>
#include <string>

#include <thread>
#include <mutex>

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H





class messageHandler
{
public:
    messageHandler();
    bool sendMessage();

    volatile bool thread_receiving=false;
    volatile bool thread_sending= false;

    void start();


    int server();

    ~messageHandler();
};

#endif // MESSAGEHANDLER_H
