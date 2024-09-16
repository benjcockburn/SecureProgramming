#include "connection.h"
#include "server.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    // Listen on all interfaces (QHostAddress::Any) and bind to port 8001
    listen(QHostAddress::Any, 8001);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    Connection *connection = new Connection(socketDescriptor, this);
    emit newConnection(connection);
}
