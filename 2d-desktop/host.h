#ifndef HOST_H
#define HOST_H

#include <QString>

class host
{
public:
    host();
    host(QString, unsigned, QString);

private:
    QString ip;
    unsigned port;
    QString nick;

public:
    void setIp(QString);
    void setPort(unsigned);
    void setNick(QString);

    QString getIp();
    unsigned getPort();
    QString getNick();

    operator QString();
};

#endif // HOST_H
