#include "socket.h"

#include <driver.h>
#include <connection.h>
#include <prepared_statement.h>
#include <resultset.h>
#include <exception.h>

socket::socket(QObject* parent) : QUdpSocket(parent), me("", 0, "")
{
    this->bind(QHostAddress::Any, 4000);

    me.setIp(localAddress().toString());
    me.setPort(localPort());
    me.setNick("m_server");

    qDebug() << "SERVER HOST:" << me;
}

void socket :: incomming()
{
    qDebug() << "Message from someone else ...";

    QByteArray bt;
    QDataStream str(&bt, QIODevice::ReadOnly);

    do
    {
        bt.resize(this->pendingDatagramSize());
        this->readDatagram(bt.data(), bt.size());
    }
    while(hasPendingDatagrams());

    QString gethost;
    QString message;

    str >> gethost >> message;

    host outer("", 0, "");

    QStringList hst = gethost.split(" ");
    outer.setIp(hst.at(0));
    outer.setPort(hst.at(1).toUInt());
    outer.setNick(hst.at(2));

    using namespace sql;

    if(message == "loggin")
    {
        if(mp[outer.getNick()] == 0)
            mp[outer.getNick()] = 3;

        try
        {
            Driver* dr = get_driver_instance();
            Connection* con = dr->connect("tcp://localhost", "root", "");

            con->setSchema("2d_im");

            PreparedStatement* stmt = con->prepareStatement("select nick from usuario where nick = ?");
            stmt->setString(1, outer.getNick().toStdString());

            ResultSet* rs = stmt->executeQuery();

            int i = 0;

            while(rs->next())
                i++;

            //

            QByteArray btSend;
            QDataStream strSend(&btSend, QIODevice::WriteOnly);

            //

            if(i == 0)
            {
                qDebug() << "User does not exist ...";

                if(mp[outer.getNick()] == 1) // Não pode ser 0, uma vez que todos os elementos são zerados inicialmente ...
                    strSend << QString("null") << QString("exit");
                else
                {
                    mp[outer.getNick()]--;
                    strSend << QString("null") << QString("%1").arg(mp[outer.getNick()]);
                }
            }
            else
            {
                if(indexOfHost(outer) != -1)
                    strSend << QString("just");
                else
                {
                    QList<host> onlinefriends;
                    qDebug() << "User exists ...";
                    // Veririficar password ...
                    QString getpass;
                    str >> getpass; // Avança com o password ...

                    qDebug() << "PASSWORD: " << getpass;

                    QString retrieved("");

                    PreparedStatement* stmt = con->prepareStatement("select pass from usuario where nick = ?");
                    stmt->setString(1, outer.getNick().toStdString());

                    ResultSet* rs = stmt->executeQuery();

                    while(rs->next())
                        retrieved = rs->getString("pass").c_str();

                    qDebug() << "Retornado: " << retrieved;

                    if(retrieved == getpass)
                    {
                        qDebug() << "Password good ...";
                        // Registar o usuário no servidor, retornar a lista dos amigos, comparando com os que estão online e enviando msgs aos outros onlines ...
                        onlines.append(outer);

                        PreparedStatement* stmt = con->prepareStatement("select added from amizade where adder = ?"); // PESQUISA POR ADDED ...
                        stmt->setString(1, outer.getNick().toStdString());

                        ResultSet* rs = stmt->executeQuery();

                        // FRIENDS ...
                        QStringList allfriends;

                        while(rs->next())
                            allfriends.append(rs->getString("added").c_str());

                        stmt = con->prepareStatement("select adder from amizade where added = ?"); // PESQUISA POR ADDED ...
                        stmt->setString(1, outer.getNick().toStdString());

                        rs = stmt->executeQuery();

                        while(rs->next())
                            allfriends.append(rs->getString("adder").c_str());

                        qDebug() << "Your friends:" << allfriends;

                        foreach (QString getter, allfriends) {
                            foreach (host get, onlines) {
                                if(getter == get.getNick())
                                    onlinefriends.append(get);
                            }
                        }

                        QStringList lista;

                        foreach (host get, onlinefriends) {
                            lista.append(static_cast<QString>(get));
                        }

                        strSend << QString("friends") << lista;

                        stmt = con->prepareStatement("insert into sessao values(?, now(), 1)");
                        stmt->setString(1, outer.getNick().toStdString());

                        stmt->execute();

                        // Manda mensagens de entrada aos amigos online (se existirem) ...
                        QByteArray tempBt;
                        QDataStream tempDst(&tempBt, QIODevice::WriteOnly);

                        tempDst << me << QString("user_in") << outer;

                        foreach(host get, onlinefriends)
                        {
                            qDebug() << "SEND MESSAGE TO:" << get.getNick() << ":" << get.getIp() << ":" << get.getPort();
                            qDebug() << "STATUS:" << writeDatagram(tempBt, QHostAddress(get.getIp()), get.getPort());
                        }
                    }
                    else
                    {
                        qDebug() << "Password error ...";

                        if(mp[outer.getNick()] == 1) // Não pode ser 0, uma vez que todos os elementos sao zerados inicialmente ...
                            strSend << QString("password_error") << QString("exit");
                        else
                        {
                            mp[outer.getNick()]--;
                            strSend << QString("password_error") << QString("%1").arg(mp[outer.getNick()]);
                        }
                    }
                }
            }

            writeDatagram(btSend, QHostAddress(outer.getIp()), outer.getPort());
            qDebug() << "Sent back ...";

            con->close();
        }
        catch(SQLException excp)
        {
            qDebug() << "MySQL Exception:" << excp.what();
        }

        qDebug() << "Connection done ...";
    }
    else
    {
        if(message == "logout")
        {
            try
            {
                Driver* drv = get_driver_instance();
                Connection* con = drv->connect("tcp://localhost", "root", "");

                con->setSchema("2d_im");

                PreparedStatement* stmt = con->prepareStatement("insert into sessao values(?, now(), 0)");
                stmt->setString(1, outer.getNick().toStdString());

                stmt->execute();
            }
            catch(SQLException excp)
            {
                qDebug() << "MySQL Exception:" << excp.what();
            }

            qDebug() << gethost << "Deseja sair [" << indexOfHost(outer) << "]";
            onlines.removeAt(indexOfHost(outer));
        }
        else
        {
            if(message == "chport")
            {
                qDebug() << gethost << "Quer mudar de porta";

                qDebug() << "Para:" << outer.getPort();

                host temp = onlines.at(indexOfHost(outer));
                temp.setPort(outer.getPort());

                onlines.replace(indexOfHost(outer), temp); // Isso tudo para mudar a porta só :D ...

                foreach (host get, onlines) {
                    qDebug() << "HOSTS:" << get;
                }
            }
            else
            {
                // Outros ...

                if(message == "revoke")
                {
                    qDebug() << "Revoked";
                        mp[outer.getNick()] = 0;
                }
            }
        }
    }

    qDebug() << "Read done ...";
}

int socket :: indexOfHost(host arg)
{
    qDebug() << "Temos:" << onlines.size() << "usuários ...";
    int i = 0;

    foreach(host get, onlines)
    {
        if(get.getNick() == arg.getNick())
            return i;
        i++;
    }

    return -1;
}
