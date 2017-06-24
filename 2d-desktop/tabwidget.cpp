#include "tabwidget.h"

tabwidget::tabwidget(QWidget *parent) :
    QTabWidget(parent)
{
    setTabsClosable(true);
}

void tabwidget :: add(abstracttab* item, QString nick)
{
    addTab(item, nick);
    tabs.append(item);
}
