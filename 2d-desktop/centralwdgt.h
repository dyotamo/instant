#ifndef CENTRALWDGT_H
#define CENTRALWDGT_H

#include <QWidget>

#include <QTabWidget>
#include <QListWidget>

#include <QSplitter>

#include <QPushButton>

#include "listwidget.h"
#include "tabwidget.h"

class CentralWdgt : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWdgt(QWidget *parent = 0);

signals:

public slots:

public:
    tabwidget* tabs;
    listwidget* buddywidget;

    QSplitter* div;

    QPushButton* btnQuit;

private:
    void initComps();
};

#endif // CENTRALWDGT_H
