#include "client.h"
#include "connection.h"
#include "peermanager.h"

#include <QHostInfo>

#include <algorithm>
#include <functional>

Client::Client()
    : peerManager(new PeerManager(this))
{
    peerManager->setServerPort(server.serverPort());
    peerManager->startBroadcasting();

    connect(peerManager, &PeerManager::newConnection,
            this, &Client::newConnection);
    connect(&server, &Server::newConnection,
            this, &Client::newConnection);
}

void Client::sendMessage(const QString &message)
{
    if (message.isEmpty())
        return;

    for (Connection *connection : std::as_const(peers))
        connection->sendMessage(message);
}


void Client::sendMessageDirectly(const QString &message, const QString &address, quint16 port)
{
    if (message.isEmpty())
        return;

    // Create a new connection
    Connection *connection = new Connection();

    // Connect to the specified host and port
    if (!connection->connectToHost(address, port)) {
        qWarning() << "Failed to connect to host: " << address << "on port:" << port;
        delete connection;
        return;
    }

    // Send the message to the connected host
    connection->sendMessage(message);

    // Clean up the connection (optional: depending on your usage, you might want to delete it)
    delete connection;
}



QString Client::nickName() const
{
    return peerManager->userName() + '@' + QHostInfo::localHostName()
           + ':' + QString::number(server.serverPort());
}

bool Client::hasConnection(const QByteArray &peerUniqueId) const
{
    return peers.contains(peerUniqueId);
}

void Client::newConnection(Connection *connection)
{
    connection->setGreetingMessage(peerManager->userName(), peerManager->uniqueId());
    connect(connection, &Connection::readyForUse, this, &Client::readyForUse);
    connect(connection, &Connection::errorOccurred, connection, &QObject::deleteLater);
    connect(connection, &Connection::disconnected, connection, &QObject::deleteLater);
}

void Client::readyForUse()
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (!connection || hasConnection(connection->uniqueId())) {
        if (connection) {
            connection->disconnectFromHost();
            connection->deleteLater();
        }
        return;
    }

    connect(connection, &Connection::errorOccurred, this, &Client::connectionError);
    connect(connection, &Connection::disconnected, this, &Client::disconnected);
    connect(connection, &Connection::newMessage, this, &Client::newMessage);

    peers.insert(connection->uniqueId(), connection);
    QString nick = connection->name();
    if (!nick.isEmpty())
        emit newParticipant(nick);
}

void Client::disconnected()
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::connectionError(QAbstractSocket::SocketError /* socketError */)
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::removeConnection(Connection *connection)
{
    if (peers.remove(connection->uniqueId())) {
        QString nick = connection->name();
        if (!nick.isEmpty())
            emit participantLeft(nick);
    }
    connection->deleteLater();
}