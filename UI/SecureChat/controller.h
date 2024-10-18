//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "JsonHandler.h"
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
class controller_sp
{
public:
    controller_sp(JsonHandler * jsonInput,int port);

    JsonHandler * jsonHandler;

        bool start();
        int port;
        volatile bool thread_receiving_controller = false;
        int server();

        


        ~controller_sp();

        private:
            int result = -1;
};

#endif // CONTROLLER_H
