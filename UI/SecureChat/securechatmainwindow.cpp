#include "securechatmainwindow.h"
#include "RSA_generate_Keys.h"

#include "./ui_securechatmainwindow.h"

SecureChatMainWindow::SecureChatMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SecureChatMainWindow)
{
    ui->setupUi(this);

    this->ui->tabWidget->setCurrentIndex(0);
    this->ui->tabWidget->setTabEnabled(1, false);
    this->ui->tabWidget->setTabEnabled(2, false);
    handler = new messageHandler(this);

    bool outcome = handler->start();
    std::cout << outcome << std::endl;


    jsonHandler = new JsonHandler();

    

    this->controller = new controller_sp(jsonHandler, this->port);
bool anotherOutcome = controller->start();
    connect(handler, &messageHandler::messageReceived, this, &SecureChatMainWindow::DisplayMessage);

    // add fake people!
    // this->addRecipient("192.168.0.239:12345",loadPublicKey("/Users/ben/Library/CloudStorage/OneDrive-UniversityofAdelaide/Work/Uni/YEAR-3-SEM-2/Secure_Programming/SecureProgramming/public_key_example_2.pem") );
    // this->addRecipient("192.168.0.179:12345",loadPublicKey("/Users/ben/Library/CloudStorage/OneDrive-UniversityofAdelaide/Work/Uni/YEAR-3-SEM-2/Secure_Programming/SecureProgramming/public_key_example_1.pem") );

    on_python_tryconnect_clicked();

    if (!outcome || !anotherOutcome)
    {
        this->ui->tabWidget->setTabEnabled(2, true);
        this->ui->tabWidget->setTabEnabled(0, false);
        // this->close();
        this->ui->errorLabel->setText("PORTS FAILED TO BIND, PLEASE CLOSE THE OTHER INSTANCE OF THIS APPLICATION\n\nTo run multiple clients you need a seperate computer, sorry fixing this would've taken weeks\n\nTip: best connection is between a mac-linux or linux-linux or mac-mac, as WSL doesnt easily allow for ports to communicate both ways");
        std::cout << "Trying to quit"<<std::endl;
        return;
    }
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
    QString recipient_value;

    sender = this->myself->name;

    if (this->ui->message_receipients->currentText() == "PUBLIC (to all users online)")
    {

        // TODO: add in correct reciepient
        nlohmann::json JsonOutput = jsonHandler->constructPublicChat(myself->fingerprint, chat_message.toStdString().c_str());

        std::string jsonString = JsonOutput.dump();

        recipient_value = QString("Public");
        handler->sendMessage(jsonString.c_str(), this->port);
    }
    else
    {
        std::string recipient_finger = this->ui->message_receipients->currentText().toStdString();
        recipient_value = QString(recipient_finger.c_str());
        // recipient target =

        std::string key = recipient_finger;

        auto it = list.find(key);

        // Check if key was found
        if (it != list.end())
        {
            std::cout << "Found " << std::endl;
        }
        else
        {
            std::cout << "'" << key << "' not found in the map." << std::endl;
        }

        recipient where = it->second;

        std::vector<std::string> dest_servers;

        dest_servers.push_back(where.dest_server);

        std::cout << "dest_servers: " << where.dest_server << std::endl;

        std::vector<std::string> fingerprints;

        fingerprints.push_back(where.fingerPrint);

        std::cout << "fingerprints: " << where.fingerPrint << std::endl;

        std::vector<std::string> keys;

        keys.push_back(where.PublicKeyString());

        std::cout << "keys: " << where.PublicKeyString() << std::endl;

        nlohmann::json JsonOutput = jsonHandler->constructChat(dest_servers, fingerprints, chat_message.toStdString(), keys);

        std::string jsonString = JsonOutput.dump();

        handler->sendMessage(jsonString.c_str(), this->port);
    }

    // QT VISUALS STUFF
    QString formatted_message = formatMessage(chat_message, recipient_value, sender);
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

    std::cout << "message: " << message.toStdString() << std::endl;

    if(jsonHandler->validateMessage(message.toStdString())){
        return;
    }

    std::string message_str;
    std::string sender_dis;
    std::string recipient_dis;
    nlohmann::json jsonObject = nlohmann::json::parse(message.toStdString());

    if (jsonObject["data"]["type"] == "chat")
    {
        RSA *private_key_read = this->myself->PrivateKey;

        //loadPrivateKeyFromFile("/Users/ben/Library/CloudStorage/OneDrive-UniversityofAdelaide/Work/Uni/YEAR-3-SEM-2/Secure_Programming/SecureProgramming/private_key_example_2.pem");

        if (private_key_read)
        {
            nlohmann::json message_json = jsonHandler->decryptChat(jsonObject, private_key_read);
            message_str = message_json["message"];
            if (message_json.contains("message"))
            {

                std::cout << "decrypt: " << message_str << std::endl;
            }
            else
            {
                std::cerr << "Error: 'message' key not found in decrypted JSON." << std::endl;
            }
            sender_dis = jsonObject["data"]["iv"];
            recipient_dis = myself->name.toStdString();
        }
        else
        {
            std::cerr << "Error: Could not load private key." << std::endl;
        }
    }
    else if (jsonObject["data"]["type"] == "public_chat")
    {
        message_str = jsonObject["data"]["message"];
        sender_dis = jsonObject["data"]["sender"];
        recipient_dis = "public_chat";
    }
    else
    {
        std::cerr << "Error: Unknown chat type." << std::endl;
    }

    QString formatted_message = formatMessage(message_str.c_str(), recipient_dis.c_str(), sender_dis.c_str());
    this->ui->plainTextEdit->appendPlainText(formatted_message);
}

void SecureChatMainWindow::on_pushButton_clicked()
{
    // load the key in /Keys/public_key.pem

    // TODO: make sure python is on before allowing the button to be pressed

    RSA *private_key_read = generate_private_key();

    // private key
    RSA *public_key_read = generate_public_key(private_key_read);

    std::cout<<rsaPublicKeyToString(public_key_read)<<std::endl;

    // set nickname
    QString nickname = this->ui->nickname_lineedit->text();
    if (nickname == "")
    {
        nickname = QString("YOU"); // enter the key here;
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



    // Get the current text from the serverList
    QString currentText = this->ui->serverList->toPlainText();

    // Create the new entry (ip:port)
    QString newEntry = ip + ":" + port;

    // Split the current text into individual lines
    QStringList lines = currentText.split("\n", Qt::SkipEmptyParts);

    // Check if the new entry already exists
    if (!lines.contains(newEntry)) {
        // If not, append it
        this->ui->serverList->appendPlainText(newEntry);
        // Correct JSON structure
        nlohmann::json JsonOutput = {
                                     {"data", {{"type", "connect"}, {"ip", ipStr}, {"port", portStr}}}};

        std::string jsonString = JsonOutput.dump();

        handler->sendMessage(jsonString.c_str(), this->port);
    } else {
        std::cout<<"ALREADY TRIED TO CONNECT TO THAT"<<std::endl;
    }
}

void SecureChatMainWindow::addRecipient(std::string server, RSA *PublicRSA)
{

    recipient created(server, PublicRSA);

    this->ui->message_receipients->addItem(QString(created.fingerPrint.c_str()));

    list[created.fingerPrint] = created;

    // list.add(created);
};

void SecureChatMainWindow::on_nickname_lineedit_returnPressed()
{
    on_pushButton_clicked();
}

void SecureChatMainWindow::on_python_tryconnect_clicked()
{

    nlohmann::json JsonOutput = {
        {"data", {
                     {"type", "awwake"},
                 }}};

    std::string jsonString = JsonOutput.dump();

    bool result = handler->sendMessage(jsonString.c_str(), this->port);
    std::cout << "result: " << result << std::endl;

    if (result)
    {
        this->ui->pushButton->setEnabled(true);
        // this->ui->python_tryconnect->setEnabled(false);
        this->ui->python_status->setText("Python is Running");
    }
    else
    {
        this->ui->pushButton->setEnabled(false);
        this->ui->python_tryconnect->setEnabled(true);
        this->ui->python_status->setText("Python is NOT RUNNING, NO LOAD KEY");
    }
}

void SecureChatMainWindow::on_otheradd_clicked()
{
    QString key = this->ui->otherskey->text();
    QString ip = this->ui->othersip->text();

    this->addRecipient(ip.toStdString(), stringToRsaPublicKey(key.toStdString()));
}
