#ifndef CONNECTION_H
#define CONNECTION_H

#include <QCborStreamReader>
#include <QCborStreamWriter>
#include <QElapsedTimer>
#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTimer>

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    enum ConnectionState {
        WaitingForGreeting,
        ReadingGreeting,
        ProcessingGreeting,
        ReadyForUse
    };
    enum DataType {
        PlainText,
        Ping,
        Pong,
        Greeting,
        Undefined
    };

    explicit Connection(QObject *parent = nullptr);
    explicit Connection(qintptr socketDescriptor, QObject *parent = nullptr);
    ~Connection();

    QString name() const;
    void setGreetingMessage(const QString &message, const QByteArray &uniqueId);
    bool sendMessage(const QString &message);

    QByteArray uniqueId() const;


    bool connectToHost(const QString &address, quint16 port);

signals:
    void readyForUse();
    void newMessage(const QString &from, const QString &message);

protected:
    void timerEvent(QTimerEvent *timerEvent) override;

private slots:
    void processReadyRead();
    void sendPing();
    void sendGreetingMessage();

private:
    bool hasEnoughData();
    void processGreeting();
    void processData();

    QCborStreamReader reader;
    QCborStreamWriter writer;
    QString greetingMessage = tr("undefined");
    QString username = tr("unknown");
    QTimer pingTimer;
    QElapsedTimer pongTime;
    QString buffer;
    QByteArray localUniqueId;
    QByteArray peerUniqueId;
    ConnectionState state = WaitingForGreeting;
    DataType currentDataType = Undefined;
    int transferTimerId = -1;
    bool isGreetingMessageSent = false;
};

#endif
