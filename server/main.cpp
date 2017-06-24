#include <QCoreApplication>

#include "socket.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    socket sck(&a);

    QObject::connect(&sck, SIGNAL(readyRead()), &sck, SLOT(incomming()));

    return a.exec();
}
