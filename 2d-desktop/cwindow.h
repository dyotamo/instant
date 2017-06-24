#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

#include <QUdpSocket>

#include "centralwdgt.h"
#include "host.h"

#include <tuple>

#include <QDateTime>

#include <QTextEdit>
#include <QAction>
#include <QMenu>

#include <QMenuBar>

#include <map>

#include "grupodialog.h"

using std :: map;

using BufferTuple = std :: tuple<QString, QDateTime, QString>;

class cwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit cwindow(QWidget *parent, host, QList<host>);
    ~cwindow();
    void closeEvent(QCloseEvent*);
    void keyPressEvent(QKeyEvent*);

    void newPort();
    int indexOfHost(host);

    void flush();
    void retrieveHist(QString);

    QTextEdit *getBuddyTextEdit(QString);

signals:

public slots:
    void dispatchMsgs();
    void initChat(QListWidgetItem*);
    void closetab();
    void createPopup();
    void chmsg();
    void onMenu(QAction*);

private:
    CentralWdgt* win;
    host me;

    QList<host> friends;
    QUdpSocket sock;

    map<QString, QAction*> actions;
    map<QString, QMenu*> menus;

public:
    bool isChatActived(QString);
    void selectTab(QString);

    QList<host>* getFriends();

    friend class grupodialog;

public:
    QList<BufferTuple> msgs;
};

#endif // WINDOW_H
