#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include "ui_chatdialog.h"
#include "client.h"

class ChatDialog : public QDialog, private Ui::ChatDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);

public slots:
    void appendMessage(const QString &from, const QString &message);

private slots:
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void showInformation();

    void on_input_register_clicked();

    void on_input_login_clicked();

private:
    Client client;
    QString myNickName;
    QTextTableFormat tableFormat;
};

#endif
