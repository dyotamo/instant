#include "pvt.h"

#include <QUdpSocket>
#include <QDateTime>

#include <QObject>

pvt::pvt(tabwidget* parent, host i, host frnd, QList<BufferTuple>& ref) : chat(parent, "pvt"), me(i), m_host(frnd)
{
    msgs = &ref;
    connect(btnSend, SIGNAL(clicked()), this, SLOT(onSend()));
    connect(msg, SIGNAL(returnPressed()), this, SLOT(onSend()));
}

void pvt :: onSend()
{
    QString text = msg->text();

    if(!text.isEmpty())
    {
        QUdpSocket sock;
        QByteArray a;
        QDataStream s(&a, QIODevice::WriteOnly);

        qDebug() << "Myself:" << me;
        qDebug() << "Friend:" << m_host;

        s << me << QString("msg") << text;

        qDebug() << "QUA:" << sock.writeDatagram(a, QHostAddress(m_host.getIp()), m_host.getPort());

        txts->append(QString("<font color=#832B8E><b>Me</b></font> - %1: %2").arg(QTime::currentTime().toString()).arg(msg->text()));
        msg->clear();

        msgs->append(BufferTuple(m_host.getNick(), QDateTime::currentDateTime(), text));

        if(msgs->size() == 10)
        {
            qDebug() << "MUST FLUSH ...";

            cwindow* prnt = reinterpret_cast<cwindow*>(reinterpret_cast<CentralWdgt*>((reinterpret_cast<tabwidget*>(parent())->parent())->parent())); // Não extensível, mas funciona, e bem mesmo ...
            prnt->flush();
        }
    }
}
