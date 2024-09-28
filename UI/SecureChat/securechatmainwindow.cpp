#include "securechatmainwindow.h"

#include "./ui_securechatmainwindow.h"

SecureChatMainWindow::SecureChatMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SecureChatMainWindow)
{
    ui->setupUi(this);

    this->ui->tabWidget->setCurrentIndex(0);
    this->ui->tabWidget->setTabEnabled(1, false);
    this->ui->tabWidget->setTabEnabled(2, false);
    handler = new messageHandler(this);

    jsonHandler = new JsonHandler();

    this->controller = new controller_sp(jsonHandler, this->port);

    connect(handler, &messageHandler::messageReceived, this, &SecureChatMainWindow::DisplayMessage);

    // add fake people!
    this->addRecipient("192.168.0.239", stringToRsaPublicKey("-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0Htkylt7i1s2ZTkc0RVT\n25l2pR/HxsxBfAXRkUt9djgBMyy7VJo02v2LvirbKkc+5U7SoBpx0F37s2UF4tD5\nvHN8AsC2GHsIAKHpO87ZLi3mAFdoVu0zGhsk3VnEe+YrsdGPC9uuCTzl6JuKS3qB\nHAfhFiQiEZO0ykWeJhI1A97eHoA0Ed3GGUJArZ43hn9enOcU0lWhP/8NxeZSbZdD\n237L8cBijVgSzc23Bc6/ye7+sI+irg9s+TsmW3i/3hZnKrxeQCBQf1ZqLKoTllO8\nxfxzn+Pvk/mqx+vmBzD4mqMWayWtORwb9vNjXMJrd31yWNMd4JmabLtOaA6wK8nu\n6wIDAQAB\n-----END PUBLIC KEY-----\n"));

    
}

SecureChatMainWindow::~SecureChatMainWindow()
{
    delete ui;
}

void SecureChatMainWindow::on_SendMessage_button_clicked()
{

    QString chat_message = this->ui->message_text_box->text();
    if (chat_message.length() == 0)
    {
        return;
    }
    QString sender;
    QString recipient;

    sender = this->myself->name;

    if (this->ui->message_receipients->currentText() == "PUBLIC (to all users online)")
    {

        // TODO: add in correct reciepient
        nlohmann::json JsonOutput = jsonHandler->constructPublicChat(myself->fingerprint, chat_message.toStdString().c_str());

        std::string jsonString = JsonOutput.dump();

        recipient = QString("Public");
        handler->sendMessage(jsonString.c_str(), this->port + 2);
    }
    else
    {
        std::string recipient_finger = this->ui->message_receipients->currentText().toStdString();

        // recipient target =





        std::vector<std::string> dest_servers;

        dest_servers.push_back(this->list.front().dest_server);

        std::vector<std::string> fingerprints;

        fingerprints.push_back(this->list.front().fingerPrint);

        std::vector<std::string> keys;

        keys.push_back(this->list.front().PublicKeyString());



        nlohmann::json JsonOutput = jsonHandler->constructChat(dest_servers ,fingerprints,"testing",keys);

        std::string jsonString = JsonOutput.dump();

        handler->sendMessage(jsonString.c_str(), this->port + 2);
    }

    // QT VISUALS STUFF
    QString formatted_message = formatMessage(chat_message, recipient, sender);
    this->ui->plainTextEdit->appendPlainText(formatted_message);
    this->ui->message_text_box->clear();

    // nlohmann::json JsonOutput = jsonHandler->constructChat("127.0.0.1",{"123"}, chat_message.toStdString().c_str(),rsaPublicKeyToString(myself->PublicKey))

    // std::string jsonString = JsonOutput.dump();

    // this->handler->sendMessage();
}

void SecureChatMainWindow::on_message_text_box_returnPressed()
{
    on_SendMessage_button_clicked();
}

QString SecureChatMainWindow::formatMessage(QString text, QString recipient, QString sender)
{
    QString formatted_message = sender + QString(" -> ") + recipient + QString(" :: ") + text;

    return formatted_message;
}

void SecureChatMainWindow::DisplayMessage(QString message, QString recipient, QString sender)
{

    QString formatted_message = formatMessage(message, recipient, sender);
    this->ui->plainTextEdit->appendPlainText(formatted_message);
}

void SecureChatMainWindow::on_pushButton_clicked()
{
    // load the key in /Keys/public_key.pem

    // TODO: make sure python is on before allowing the button to be pressed

    QString path = this->ui->key_path_lineedit->text();

    std::cout << path.toStdString() << "/public_key.pem" << std::endl;

    RSA *public_key_read = loadPublicKey(path.toStdString() + "/public_key.pem");

    // private key

    RSA *private_key_read = loadPrivateKeyFromFile(path.toStdString() + "/private_key.pem");

    std::cout << path.toStdString() << "/private_key.pem" << std::endl;

    // set nickname
    QString nickname = this->ui->nickname_lineedit->text();
    if (nickname == "")
    {
        nickname = QString("N/A"); // enter the key here;
    }

    // TODO: if any of these fail it should prevent them continuing

    myself = new client(nickname);
    myself->setPublicKey(public_key_read);
    myself->setPrivateKey(private_key_read);

    // server_hello

    nlohmann::json JsonOutput = jsonHandler->constructHello(myself->PublicKeyString());

    std::string jsonString = JsonOutput.dump();

    handler->sendMessage(jsonString.c_str(), this->port);

    this->ui->keydisplay->setText(QString(myself->PublicKeyString().c_str()));

    this->ui->nicknamedisplay->setText(QString("Logged on as: ") + nickname);

    this->ui->tabWidget->setTabEnabled(1, true);  // enable chat
    this->ui->tabWidget->setCurrentIndex(1);      // force switch to chat
    this->ui->tabWidget->setTabEnabled(0, false); // disable login
}

void SecureChatMainWindow::on_try_attempt_clicked()
{

    QString port = this->ui->port_attempt->text();

    QString ip = this->ui->ip_attempt->text();

    // Convert QString to std::string
    std::string portStr = port.toStdString();
    std::string ipStr = ip.toStdString();

    // Correct JSON structure
    nlohmann::json JsonOutput = {
        {"data", {{"type", "connect"}, {"ip", ipStr}, {"port", portStr}}}};

    std::string jsonString = JsonOutput.dump();

    handler->sendMessage(jsonString.c_str(), this->port);
}

void SecureChatMainWindow::addRecipient(std::string server, RSA *PublicRSA)
{

    recipient created(server, PublicRSA);

    this->ui->message_receipients->addItem(QString(created.fingerPrint.c_str()));

    list.push_back(created);
};

void SecureChatMainWindow::on_nickname_lineedit_returnPressed()
{
    on_pushButton_clicked();
}
