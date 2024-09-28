#ifndef SECURECHATMAINWINDOW_H
#define SECURECHATMAINWINDOW_H
#include <iostream>
#include <QMainWindow>
#include "messagehandler.h"
#include "client.h"
#include "decrypt_RSA.h"
#include "encrypt_AES.h"
#include "JsonHandler.h"
#include "controller.h"
#include "recipient.h"



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

    void addRecipient(std::string server,RSA * PublicRSA);

private slots:

    void on_SendMessage_button_clicked();

    void on_message_text_box_returnPressed();

    void on_pushButton_clicked();

    void on_try_attempt_clicked();

    void on_nickname_lineedit_returnPressed();

private:
    Ui::SecureChatMainWindow *ui;
    messageHandler * handler;
    client * myself;
    JsonHandler * jsonHandler;
    int port = 8000;
    controller_sp * controller;
    std::vector<recipient> list;

    

    QString formatMessage(QString text, QString recipient,QString sender);

    void DisplayMessage(QString message, QString recipient, QString sender);
};


#endif // SECURECHATMAINWINDOW_H
