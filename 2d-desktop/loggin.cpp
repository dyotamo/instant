#include "loggin.h"
#include "ui_loggin.h"

#include <QDebug>
#include <QMessageBox>

#include <QNetworkInterface>

#include <QDesktopServices>
#include <QUrl>

#include "host.h"

#include "cwindow.h"

loggin::loggin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loggin),
    sock(new QUdpSocket(this))
{
    ui->setupUi(this);

    connect(ui->btnExt, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnEnt, SIGNAL(clicked()), this, SLOT(attemptLoggin()));
    connect(ui->labelEsq, SIGNAL(linkActivated(QString)), this, SLOT(gotoLink(QString)));
    connect(ui->labelReg, SIGNAL(linkActivated(QString)), this, SLOT(gotoLink(QString)));
    connect(ui->lineID, SIGNAL(textChanged(QString)), this, SLOT(lowerCase(QString)));

    connect(this, SIGNAL(rejected()), this, SLOT(close()));

    srand(time(0));
    prt = 1024 + rand() % 64512;

    sock->bind(QHostAddress::Any, prt);
}

loggin::~loggin()
{
    delete ui;
}

void loggin :: attemptLoggin()
{
    qDebug() << "toLoggin ...";

    QString getnick = ui->lineID->text();
    QString getpass = ui->linePass->text();

    if(getnick.isEmpty() || getpass.isEmpty())
        QMessageBox::critical(this, "Exception", "Nem todos os campos foram preenchidos", QMessageBox::Ok);
    else
    {
        QList<QHostAddress> getints = QNetworkInterface::allAddresses();
        QString ipAddress("");

        for(int i = 0; i < getints.size(); i++)
        {
            if (getints.at(i) != QHostAddress::LocalHost &&
                    getints.at(i).toIPv4Address()) {
                ipAddress = getints.at(i).toString();
                break;
            }
        }

        if(ipAddress == "")
            ipAddress = "localhost";

        qDebug() << "IP:" << ipAddress;

        host me(ipAddress, sock->localPort(), getnick);

        QByteArray bt;
        QDataStream str(&bt, QIODevice::WriteOnly);

        str << me << QString("loggin") << QString(getpass);

        sock->writeDatagram(bt, QHostAddress::LocalHost, 4000);

        if(sock->waitForReadyRead(10000))
        {
            qDebug() << "Message returned ...";

            QString servmsg;

            QByteArray bt;
            QDataStream dst(&bt, QIODevice::ReadOnly);

            do
            {
                bt.resize(sock->pendingDatagramSize());
                sock->readDatagram(bt.data(), bt.size());
            }
            while(sock->hasPendingDatagrams());

            dst >> servmsg;

            qDebug() << servmsg;

            if(servmsg == "null")
            {
                QString extra;

                dst >> extra;

                if(extra == "exit")
                {
                    QMessageBox::critical(this, "Exception", QString("%1 não existe, aplicação fechando ...").arg(getnick), QMessageBox::Ok);
                    close();
                }
                else
                    QMessageBox::critical(this, "Exception", QString("%1 não existe, faltam %2 tentativa(s) ...").arg(getnick).arg(extra), QMessageBox::Ok);
            }
            else
                if(servmsg == "password_error")
                {
                    QString extra;
                    dst >> extra;

                    if(extra == "exit")
                    {
                        QMessageBox::critical(this, "Exception", "Password errado, aplicação fechando ...", QMessageBox::Ok);
                        close();
                    }
                    else
                        QMessageBox::critical(this, "Exception", QString("Password errado, faltam %1 tentativa(s) ...").arg(extra), QMessageBox::Ok);
                }
                else
                {
                    if(servmsg == "just")
                        QMessageBox::critical(this, "Exception", QString("%1 já está online ...").arg(getnick), QMessageBox::Ok);
                    else
                    {
                        QList<host> friends;

                        QStringList lista;
                        dst >> lista;

                        // Process the online friends list ...

                        host buffer("", 0, "");

                        QStringList inner;

                        foreach (QString get, lista) {
                            inner = get.split(" ");
                            buffer.setIp(inner.at(0));
                            buffer.setPort(inner.at(1).toUInt());
                            buffer.setNick(inner.at(2));

                            friends.append(buffer);
                        }

                        qDebug() << "Amigos online: " << friends.size();

                        cwindow* wnd = new cwindow(0, host(ipAddress, sock->localPort(), getnick), friends);
                        wnd->show();

                        close();
                    }
                }

        }
        else
            QMessageBox::critical(this, "Exception", "Não é possivel manter conexão com o servidor ...", QMessageBox::Ok);
    }
}

void loggin :: gotoLink(QString arg)
{
    qDebug() << "Follow:" << arg;
    QDesktopServices::openUrl(QUrl(arg));
}

void loggin :: lowerCase(QString arg)
{
    ui->lineID->setText(arg.toLower());
}

void loggin :: closeEvent(QCloseEvent*)
{
    QByteArray bt;
    QDataStream str(&bt, QIODevice::WriteOnly);

    str << host(sock->localAddress().toString(), sock->localPort(), ui->lineID->text()) << QString("revoke");

    qDebug() << "Sent:" << sock->writeDatagram(bt, QHostAddress::LocalHost, 4000);
}
