#include "securechatmainwindow.h"
#include "encrypt_AES.cpp"
#include "decrypt_RSA.cpp"

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
    //    qDebug()<< "Public Chat";

         recipient = QString("Public");

    } else {
        recipient = this->ui->message_receipients->currentText();


    }





    QString formatted_message = formatMessage(chat_message,recipient,sender);



    this->ui->plainTextEdit->appendPlainText(formatted_message);
    this->ui->message_text_box->clear();

    this->handler->sendMessage();




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

std::string RSA_to_string(RSA *rsa) {
    // Create a memory BIO
    BIO *bio = BIO_new(BIO_s_mem());
    
    // Write the RSA public key to the BIO in PEM format
    PEM_write_bio_RSA_PUBKEY(bio, rsa);

    // Get a pointer to the data in the BIO
    char *pem_key = nullptr;
    long pem_len = BIO_get_mem_data(bio, &pem_key);

    // Copy the PEM data into a C++ string
    std::string key_string(pem_key, pem_len);

    // Clean up
    BIO_free(bio);

    return key_string;
}
void SecureChatMainWindow::on_pushButton_clicked()
{
    // load the key in /Keys/public_key.pem

    QString path = this->ui->key_path_lineedit->text();

    std::cout << path.toStdString() << "/public_key.pem"<< std::endl;

    RSA* public_key_read = loadPublicKey(path.toStdString()+"/public_key.pem"); 


// private key

    RSA* private_key_read = loadPrivateKeyFromFile(path.toStdString() + "/private_key.pem");

    std::cout << path.toStdString() <<  "/private_key.pem"<< std::endl;

    // set nickname
    QString nickname = this->ui->nickname_lineedit->text();
    if(nickname==""){
        nickname = QString(RSA_to_string(public_key_read).c_str()); // enter the key here;
    }

    myself= new client(nickname);
    myself->PublicKey= public_key_read;
    myself->PrivateKey= private_key_read;

    

    this->ui->keydisplay->setText(nickname);
    this->ui->chat_tab->setEnabled(true);
    this->ui->tabWidget->setCurrentIndex(1);
    this->ui->login_tab->setEnabled(false);


}

