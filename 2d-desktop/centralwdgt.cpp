#include "centralwdgt.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

CentralWdgt::CentralWdgt(QWidget *parent) :
    QWidget(parent)
{
    initComps();
}

void CentralWdgt :: initComps()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;

    mainLayout->addWidget(div = new QSplitter(Qt::Horizontal));

    div->addWidget(tabs = new tabwidget);
    div->addWidget(buddywidget = new listwidget);

    div->setStretchFactor(0, 1);

    setLayout(mainLayout);
}
