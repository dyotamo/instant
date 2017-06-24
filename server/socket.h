#ifndef SOCKET_H
#define SOCKET_H

#include <QUdpSocket>

#include "host.h"

#include <map>

using std :: map;

class socket : public QUdpSocket
{
    Q_OBJECT
public:
    socket(QObject*);
    int indexOfHost(host);

private slots:
    void incomming();

private:
    host me;
    QList<host> onlines;

    map<QString, unsigned> mp;
};

#endif // SOCKET_H
