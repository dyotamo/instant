#include "host.h"

host :: host()
{

}

host::host(QString aip, unsigned aport, QString anick):
    ip(aip),
    port(aport),
    nick(anick)
{
}

void host :: setIp(QString arg)
{
    ip = arg;
}

void host :: setPort(unsigned arg)
{
    port = arg;
}

void host :: setNick(QString arg)
{
    nick = arg;
}

QString host :: getIp()
{
    return ip;
}

unsigned host :: getPort()
{
    return port;
}

QString host :: getNick()
{
    return nick;
}

host :: operator QString()
{
    return QString("%1 %2 %3").arg(ip).arg(port).arg(nick);
}
