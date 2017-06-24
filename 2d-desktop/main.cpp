#include "loggin.h"
#include <QApplication>

#include "cwindow.h"
#include "host.h"

#include "profile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loggin w;
    w.show();

    return a.exec();
}
