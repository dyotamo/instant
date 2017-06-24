#include "grupodialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "cwindow.h"
#include "group.h"

grupodialog::grupodialog(QWidget* parent) :
    QDialog(parent)
{
    cwindow* prnt = reinterpret_cast<cwindow*>(parent);
    getHosts = prnt->getFriends();

    initComps();

    connect(btn, SIGNAL(clicked()), this, SLOT(onCreate()));

    setModal(true);
}

void grupodialog :: initComps()
{
    QVBoxLayout* layout = new QVBoxLayout;

    layout->addWidget(frnds = new QListWidget);
    frnds->setSelectionMode(QAbstractItemView::MultiSelection);

    layout->addWidget(name = new QLineEdit);

    foreach (host get, *getHosts)
        frnds->addItem(get.getNick());

    layout->addWidget(btn = new QPushButton("&Create"));

    setLayout(layout);
}

void grupodialog :: onCreate()
{
    QString getName = name->text();

    if(getName.isEmpty())
        qDebug() << "Critical: No name ...";
    else
    {
        QList<host> partcpnts;
        QList<QListWidgetItem*> selected = frnds->selectedItems();

        foreach (QListWidgetItem* get, selected) {
           foreach(host oters, *getHosts)
           {
               if(oters.getNick() == get->text())
                   partcpnts.append(oters);
           }
        }

        prnt->win->tabs->add(new group(prnt->win->tabs, partcpnts), getName);

        sendMessage(partcpnts);

        close();
    }
}

void grupodialog :: sendMessage(QList<host>&)
{

}
