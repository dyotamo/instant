#ifndef GRUPODIALOG_H
#define GRUPODIALOG_H

#include <QDialog>

#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>

#include "host.h"

class cwindow;

class grupodialog : public QDialog
{
    Q_OBJECT
public:
    explicit grupodialog(QWidget*);

signals:

public slots:
    void onCreate();

private:
    void initComps();
    void sendMessage(QList<host>&);

private:
    QListWidget* frnds;
    QLineEdit* name;
    QPushButton* btn;

    QList<host>* getHosts;
    cwindow* prnt;
};

#endif // GRUPODIALOG_H
