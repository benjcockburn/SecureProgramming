#include "securechatmainwindow.h"
#include "messagehandler.h"

#include <QApplication>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    SecureChatMainWindow w;
    w.show();
    return a.exec();
}
