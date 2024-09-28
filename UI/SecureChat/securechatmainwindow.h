#ifndef SECURECHATMAINWINDOW_H
#define SECURECHATMAINWINDOW_H

#include <QMainWindow>
#include "messagehandler.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SecureChatMainWindow;
}
QT_END_NAMESPACE

class SecureChatMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SecureChatMainWindow(QWidget *parent = nullptr);
    ~SecureChatMainWindow();

private slots:

    void on_SendMessage_button_clicked();

    void on_message_text_box_returnPressed();

    void on_pushButton_clicked();

private:
    Ui::SecureChatMainWindow *ui;
    messageHandler * handler;
    client * myself;

    QString formatMessage(QString text, QString recipient,QString sender);

    void DisplayMessage(QString message, QString recipient, QString sender);
};


#endif // SECURECHATMAINWINDOW_H