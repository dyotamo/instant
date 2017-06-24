#include "chat.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

chat::chat(tabwidget * parent, QString type) : abstracttab(parent, type)
{
    initComps();
    connect(msg, SIGNAL(textChanged(QString)), this, SLOT(onedittext(QString)));

    btnSend->setDisabled(true);
    txts->setReadOnly(true);
}

void chat :: initComps()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;

    mainLayout->addWidget(txts = new QTextEdit(this));

    QHBoxLayout* innerLayout = new QHBoxLayout;

    mainLayout->addLayout(innerLayout);

    innerLayout->addWidget(msg = new QLineEdit);
    innerLayout->addWidget(btnSend = new QPushButton("&Enviar"));

    setLayout(mainLayout);
}

void chat :: onedittext(QString arg)
{
    btnSend->setDisabled(arg.isEmpty());
}
