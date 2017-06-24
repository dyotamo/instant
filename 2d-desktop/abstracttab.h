#ifndef ABSTRACTTAB_H
#define ABSTRACTTAB_H

#include <QWidget>

class abstracttab : public QWidget
{
public:
    abstracttab(QWidget*, QString);

protected:
    QString type;

public:
    QString gettype();
};

#endif // ABSTRACTTAB_H
