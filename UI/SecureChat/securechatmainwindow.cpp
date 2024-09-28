#include "securechatmainwindow.h"
#include "./ui_securechatmainwindow.h"

SecureChatMainWindow::SecureChatMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SecureChatMainWindow)
{
    ui->setupUi(this);

this->ui->tabWidget->setCurrentIndex(0);
    this->ui->chat_tab->setEnabled(false);

    handler = new messageHandler(this);


    connect(handler, &messageHandler::messageReceived, this, &SecureChatMainWindow::DisplayMessage);

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
    QString sender;
    QString recipient;

    sender = this->myself->name;

    if(this->ui->message_receipients->currentText()=="PUBLIC (to all users online)"){
       qDebug()<< "Public Chat";

         recipient = QString("Public");

    } else {
        recipient = this->ui->message_receipients->currentText();


    }





    QString formatted_message = formatMessage(chat_message,recipient,sender);



    this->ui->plainTextEdit->appendPlainText(formatted_message);
    this->ui->message_text_box->clear();






}


void SecureChatMainWindow::on_message_text_box_returnPressed()
{
    on_SendMessage_button_clicked();
}

QString SecureChatMainWindow::formatMessage(QString text, QString recipient,QString sender){
    QString formatted_message = sender + QString(" -> ")+recipient + QString(" :: ")+ text;

    return formatted_message;
}

void SecureChatMainWindow::DisplayMessage(QString message, QString recipient, QString sender) {

    QString formatted_message = formatMessage(message,recipient,sender);
    this->ui->plainTextEdit->appendPlainText(formatted_message);

}

void SecureChatMainWindow::on_pushButton_clicked()
{
    // generate a key

    // set nickname
    QString nickname = this->ui->nickname_lineedit->text();
    if(nickname==""){
        nickname = QString("Key"); // enter the key here;
    }

    myself= new client(nickname);

    this->ui->keydisplay->setText(nickname);
    this->ui->chat_tab->setEnabled(true);
    this->ui->tabWidget->setCurrentIndex(1);
    this->ui->login_tab->setEnabled(false);


}

