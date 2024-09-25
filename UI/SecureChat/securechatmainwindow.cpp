#include "securechatmainwindow.h"
#include "./ui_securechatmainwindow.h"

SecureChatMainWindow::SecureChatMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SecureChatMainWindow)
{
    ui->setupUi(this);

    messageHandler handler;
    // handler = new messageHandler();
    handler.sendMessage();
}

SecureChatMainWindow::~SecureChatMainWindow()
{
    delete ui;
}



void SecureChatMainWindow::on_SendMessage_button_clicked()
{
    QString chat_message = this->ui->message_text_box->text();
    if(chat_message.length()==0){
        return;
    }
    QString recipient = QString("Dave");
    QString sender = QString("bob");
    QString formatted_message = formatMessage(chat_message,recipient,sender);
    this->ui->plainTextEdit->appendPlainText(formatted_message);
    this->ui->message_text_box->clear();



}


void SecureChatMainWindow::on_message_text_box_returnPressed()
{
    on_SendMessage_button_clicked();
}

QString SecureChatMainWindow::formatMessage(QString &text, QString &recipient,QString &sender){
    QString formatted_message = sender + QString(" -> ")+recipient + QString(" :: ")+ text;

    return formatted_message;
}

