//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)

#include "securechatmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    SecureChatMainWindow w;
    w.show();
    return a.exec();
}
