#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

#include "abstracttab.h"

class tabwidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit tabwidget(QWidget *parent = 0);

signals:

public slots:

public:
    QList<abstracttab*> tabs;

public:
    void add(abstracttab*, QString);
};

#endif // TABWIDGET_H
