#ifndef LOGGIN_H
#define LOGGIN_H

#include <QDialog>

#include <QUdpSocket>

#include "host.h"

namespace Ui {
class loggin;
}

class loggin : public QDialog
{
    Q_OBJECT

public:
    explicit loggin(QWidget *parent = 0);
    ~loggin();

    void closeEvent(QCloseEvent*);

private:
    Ui::loggin *ui;

private slots:
    void attemptLoggin();
    void gotoLink(QString);
    void lowerCase(QString);

private:
    QUdpSocket* sock;
    int prt;
};

#endif // LOGGIN_H
