#include "abstracttab.h"

abstracttab::abstracttab(QWidget *parent, QString tp) : QWidget(parent), type(tp)
{
}

QString abstracttab :: gettype()
{
    return type;
}
