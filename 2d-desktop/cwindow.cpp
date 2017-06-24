#include "cwindow.h"

#include <QKeyEvent>

#include "pvt.h"
#include "tabwidget.h"

#include <QTabBar>
#include <QStatusBar>

#include <QApplication>

#include "grupodialog.h"

#include <QSound>

cwindow::cwindow(QWidget *parent, host myself, QList<host> frnds) :
    QMainWindow(parent),
    win(new CentralWdgt(this)),
    me(myself),
    friends(frnds)
{
    setCentralWidget(win);
    setWindowTitle(QString("2D - Client [%1]").arg(me.getNick()));
    resize(QSize(700, 500));

    foreach(host get, friends)
        win->buddywidget->addItem(get.getNick());

    newPort(); // Muda de porta e manda uma msg ao servidor ...
    qDebug() << "SUA PORTA ACTUAL:" << sock.localPort();

    connect(&sock, SIGNAL(readyRead()), this, SLOT(dispatchMsgs()));
    connect(win->buddywidget, SIGNAL(return_pressed(QListWidgetItem*)), this, SLOT(initChat(QListWidgetItem*)));
    connect(win->buddywidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(initChat(QListWidgetItem*)));
    connect(win->tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closetab()));
    connect(win->buddywidget, SIGNAL(right_btn_press()), this, SLOT(createPopup()));

    connect(statusBar(), SIGNAL(messageChanged(QString)), this, SLOT(chmsg()));

    chmsg();

    menus["file"] = new QMenu("File", menuBar());
    menus["edit"] = new QMenu("Edit", menuBar());
    menus["tool"] = new QMenu("Tools", menuBar());
    menus["help"] = new QMenu("Help", menuBar());

    menuBar()->addMenu(menus["file"]);
    menuBar()->addMenu(menus["edit"]);
    menuBar()->addMenu(menus["tool"]);
    menuBar()->addMenu(menus["help"]);

    // File
    actions["group"] = new QAction("Create grupo chat", menus["file"]);
    actions["group"]->setStatusTip("Create group ...");

    actions["exit"] = new QAction("Exit", menus["file"]);
    actions["exit"]->setStatusTip("Exit app ...");

    menus["file"]->addAction(actions["group"]);
    menus["file"]->addSeparator();
    menus["file"]->addAction(actions["exit"]);

    // Edit
    actions["copy"] = new QAction("Copy", menus["edit"]);
    actions["copy"]->setStatusTip("Copy the content in chat ...");

    actions["cut"] = new QAction("Cut", menus["edit"]);
    actions["cut"]->setStatusTip("Cut the content in chat ...");

    actions["paste"] = new QAction("Paste", menus["edit"]);
    actions["paste"]->setStatusTip("Paste text ...");

    actions["selall"] = new QAction("Select all", menus["edit"]);
    actions["selall"]->setStatusTip("Select all text ...");

    menus["edit"]->addAction(actions["copy"]);
    menus["edit"]->addAction(actions["cut"]);
    menus["edit"]->addSeparator();
    menus["edit"]->addAction(actions["paste"]);
    menus["edit"]->addSeparator();
    menus["edit"]->addAction(actions["selall"]);

    // Tool
    actions["find"] = new QAction("Search for friends", menus["tool"]);
    actions["find"]->setStatusTip("Look for friends ...");

    actions["profile"] = new QAction("See profile", menus["tool"]);
    actions["profile"]->setStatusTip("See your profile ...");

    actions["option"] = new QAction("Options ...", menus["tool"]);
    actions["option"]->setStatusTip("Manage the application settings ...");

    menus["tool"]->addAction(actions["find"]);
    menus["tool"]->addSeparator();
    menus["tool"]->addAction(actions["profile"]);
    menus["tool"]->addSeparator();
    menus["tool"]->addAction(actions["option"]);

    // Help
    actions["about2d"] = new QAction("About us", menus["help"]);
    actions["about2d"]->setStatusTip("Information about 2D Instant ...");

    actions["aboutqt"] = new QAction("About Qt", menus["help"]);
    actions["aboutqt"]->setStatusTip("Information about Qt ...");

    menus["help"]->addAction(actions["about2d"]);
    menus["help"]->addSeparator();
    menus["help"]->addAction(actions["aboutqt"]);

    connect(menus["file"], SIGNAL(triggered(QAction*)), this, SLOT(onMenu(QAction*)));
    connect(menus["edit"], SIGNAL(triggered(QAction*)), this, SLOT(onMenu(QAction*)));
    connect(menus["tool"], SIGNAL(triggered(QAction*)), this, SLOT(onMenu(QAction*)));
    connect(menus["help"], SIGNAL(triggered(QAction*)), this, SLOT(onMenu(QAction*)));
}

cwindow :: ~cwindow()
{
    delete win;
}

void cwindow :: closeEvent(QCloseEvent* evt)
{
    QByteArray bt;
    QDataStream dst(&bt, QIODevice::WriteOnly);

    QStringList frnds;

    foreach (host get, friends) {
        frnds.append(get);
    }

    dst << me << QString("logout") << frnds;
    sock.writeDatagram(bt, QHostAddress::LocalHost, 4000);

    QByteArray oter;
    QDataStream str(&oter, QIODevice::WriteOnly);

    str << me << QString("logout");

    foreach (host get, friends)
        qDebug() << sock.writeDatagram(oter, QHostAddress(get.getIp()), get.getPort());

    QWidget::closeEvent(evt);
}

void cwindow :: keyPressEvent(QKeyEvent* evt)
{
    if(evt->key() == Qt::Key_P)
        qDebug() << "PORT:" << sock.localPort();
    else
        if(evt->key() == Qt::Key_F)
        {
            qDebug() << "Friends: ";

            foreach(host get, friends)
                qDebug() << get;
        }

    QWidget::keyPressEvent(evt);
}

void cwindow :: dispatchMsgs()
{
    // QSound snd("../rcc/snd.wav", this);
    // snd.play();

    QByteArray bt;
    QDataStream dst(&bt, QIODevice::ReadOnly);

    do
    {
        bt.resize(sock.pendingDatagramSize());
        sock.readDatagram(bt.data(), bt.size());
    }
    while(sock.hasPendingDatagrams());

    QString h;
    QString msg;

    dst >> h >> msg;

    qDebug() << "Message:" << h << msg;

    host hst("", 0, "");
    QStringList temp = h.split(" ");

    hst.setIp(temp.at(0));
    hst.setPort(temp.at(1).toUInt());
    hst.setNick(temp.at(2));

    if(hst.getNick() == "m_server")
    {
        qDebug() << "From server ...";
        if(msg == "user_in")
        {
            // Usuário entrou ...
            qDebug() << "Amigo entrou ...";

            QString h;
            dst >> h;

            host hst("", 0, "");
            QStringList temp = h.split(" ");

            hst.setIp(temp.at(0));
            hst.setPort(temp.at(1).toUInt());
            hst.setNick(temp.at(2));

            if(isChatActived(hst.getNick()))
            {

                QTextEdit* aux = getBuddyTextEdit(hst.getNick());

                if(aux != 0)
                    aux->append(QString("<font color=#04FF00><b>%1 is online</b></font>").arg(hst.getNick()));
                else
                    qDebug() << "Problem ...";
            }

            friends.append(hst);
            statusBar()->showMessage(QString("<i>%1 entrou ...</i>").arg(hst.getNick()), 5000);

            win->buddywidget->addItem(hst.getNick());
        }
        else
        {
            // Para dados futuros ...
            if(msg == "hist")
            {

            }
            else
            {

            }
        }
    }
    else
    {
        qDebug() << "From buddy ...";

        if(msg == "msg")
        {
            statusBar()->showMessage(QString("<i>%1 enviou uma mensagem ...</i>").arg(hst.getNick()), 5000);
            QString text;

            dst >> text;

            if(isChatActived(hst.getNick()))
            {
                qDebug() << "AQUI ...";
                QTextEdit* aux = getBuddyTextEdit(hst.getNick());

                if(aux != 0)
                    aux->append(QString("<font color=#4079C1><b>%1</b></font> - %2: %3").arg(hst.getNick()).arg(QTime::currentTime().toString()).arg(text));
                else
                    qDebug() << "Problem ...";
            }
            else
            {
                retrieveHist(hst.getNick());
                win->tabs->add(new pvt(win->tabs, me, hst, msgs), hst.getNick());

                QTextEdit* aux = getBuddyTextEdit(hst.getNick());
                aux->append(QString("<font color=#4079C1><b>%1</b></font> - %2: %3").arg(hst.getNick()).arg(QTime::currentTime().toString()).arg(text));
            }

            msgs.append(BufferTuple(me.getNick(), QDateTime::currentDateTime(), hst.getNick()));

            if(msgs.size() == 10)
                flush();
        }
        else
        {
            if(msg == "group_created")
            {
                statusBar()->showMessage(QString("<i>%1 criou grupo ...</i>").arg(hst.getNick()), 5000);
            }
            else
            {
                if(msg == "group_out")
                {
                    statusBar()->showMessage(QString("<i>%1 saiu do grupo ...</i>").arg(hst.getNick()), 5000);
                }
                else
                {
                    if(msg == "logout")
                    {
                        if(isChatActived(hst.getNick()))
                        {
                            QTextEdit* aux = getBuddyTextEdit(hst.getNick());

                            if(aux != 0)
                                aux->append(QString("<font color=#FF0000><b>%1 is offline</b></font>").arg(hst.getNick()));
                            else
                                qDebug() << "Problem ...";
                        }

                        qDebug() << hst << "is out ...";
                        friends.removeAt(indexOfHost(hst));

                        win->buddywidget->clear();

                        foreach(host get, friends)
                            win->buddywidget->addItem(get.getNick());
                    }
                    else
                    {
                        if(msg == "chport")
                        {
                            qDebug() << "Mudou de porta:" << hst;

                            host temp = friends.at(indexOfHost(hst));

                            qDebug() << "Amigo dele:" << temp;

                            temp.setPort(hst.getPort());

                            friends.replace(indexOfHost(hst), temp); // Isso tudo para mudar a porta só :D ...

                            if(isChatActived(hst.getNick()))
                            {
                                QTextEdit* aux = getBuddyTextEdit(hst.getNick());
                                chat* prnt = reinterpret_cast<chat*>(aux->parent());

                                pvt* kid = dynamic_cast<pvt*>(prnt);

                                kid->m_host.setPort(hst.getPort()); // Se isso dêr certo, sou bom! ...
                            }
                        }
                    }
                }
            }
        }
    }
}

void cwindow :: newPort()
{
    qDebug() << "BIND:" << sock.bind(QHostAddress::Any, 1024 + rand() % 64512);

    QByteArray bt;
    QDataStream dst(&bt, QIODevice::WriteOnly);

    me.setPort(sock.localPort());

    dst << me << QString("chport"); // Ou sock.localPort();

    sock.writeDatagram(bt, QHostAddress::LocalHost, 4000);

    // Informa aos amigos também ...
    foreach(host get, friends)
        sock.writeDatagram(bt, QHostAddress(get.getIp()), get.getPort());
}

int cwindow :: indexOfHost(host arg)
{
    int i = 0;

    foreach(host get, friends)
    {
        if(get.getNick() == arg.getNick())
            return i;
        i++;
    }

    return -1;
}

void cwindow :: initChat(QListWidgetItem* nick)
{
    QString frnd = nick->text();
    qDebug() << "AMIGO:" << frnd;

    if(!isChatActived(frnd))
    {
        qDebug() << "To init chat with:" << nick;

        host tochat;

        foreach(host get, friends)
        {
            if(get.getNick() == nick->text())
                tochat = get;
        }

        win->tabs->add(new pvt(win->tabs, me, tochat, msgs), nick->text());
        retrieveHist(tochat.getNick());
    }

    selectTab(frnd);
}

void cwindow :: closetab()
{
    abstracttab* gettab = reinterpret_cast<abstracttab*>(win->tabs->currentWidget());
    qDebug() << gettab->gettype();

    QString get = gettab->gettype();

    int index = win->tabs->currentIndex();

    if(get == "group")
    {
        // Sair do grupo ...
    }
    else
    {
        if(get == "pvt")
            flush();
        else
        {
        }

        win->tabs->removeTab(win->tabs->currentIndex());
    }

    win->tabs->tabs.removeAt(index);
}

void cwindow :: flush()
{
    QByteArray bt;
    QDataStream str(&bt, QIODevice::WriteOnly);

    QList<BufferTuple>* gett = &msgs;

    QList<QStringList> transformed;

    foreach(BufferTuple btpl, *gett)
    {
        // TODO ...
    }
}

bool cwindow :: isChatActived(QString arg)
{
    pvt* aux = 0;

    foreach (abstracttab* get, win->tabs->tabs) {
        if(get->gettype() == "pvt")
        {
            aux = qobject_cast<pvt*>(get);

            if(aux->m_host.getNick() == arg)
                return true;
        }
    }
    return false;
}

void cwindow :: retrieveHist(QString)
{

}

QTextEdit* cwindow::getBuddyTextEdit(QString arg)
{
    foreach(abstracttab* get, win->tabs->tabs)
    {
        if(get->gettype() == "pvt")
        {
            pvt* frnd = qobject_cast<pvt*>(get);

            if(frnd->m_host.getNick() == arg)
                return frnd->txts;
        }
    }

    return 0;
}

void cwindow :: selectTab(QString arg)
{
    int index = 0;

    foreach(abstracttab* get, win->tabs->tabs)
    {
        if(get->gettype() == "pvt")
        {
            pvt* frnd = qobject_cast<pvt*>(get);

            if(frnd->m_host.getNick() == arg)
                break;

            index++;
        }
    }

    win->tabs->setCurrentIndex(index);
}

void cwindow :: createPopup()
{
    qDebug() << "Option for:" << win->buddywidget->currentItem()->text();

    QMenu* popup = new QMenu;

    QAction* profile = new QAction(QString("See %1 profile ...").arg(win->buddywidget->currentItem()->text()), popup);
    popup->addAction(profile);

    popup->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void cwindow :: chmsg()
{
    statusBar()->showMessage(QString("Pronto: amigos online -> %1").arg(friends.size()));
}

void cwindow :: onMenu(QAction* arg)
{
    QString getactiontext = arg->text();

    if(getactiontext == "About Qt")
        qApp->aboutQt();
    else
        if(getactiontext == "Exit")
            close();
        else
            if(getactiontext == "Create grupo chat")
            {
                qDebug() << "Grupo ...";

                grupodialog* dlg = new grupodialog(this);
                dlg->show();
            }
            else
                qDebug() << "Clicked:" << getactiontext;
}

QList<host>* cwindow :: getFriends()
{
    return &friends;
}
