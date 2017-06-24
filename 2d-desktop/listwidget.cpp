#include "listwidget.h"

#include <QDebug>

listwidget::listwidget(QWidget *parent) :
    QListWidget(parent)
{
}

void listwidget :: keyPressEvent(QKeyEvent* evt)
{
    switch(evt->key())
    {
    case Qt::Key_Return:
    {
        QListWidgetItem* get = currentItem();

        if(get && (get->text() != ""))
            emit return_pressed(get);
        break;
    }
    }

    QListWidget::keyPressEvent(evt);
}

void listwidget :: mouseReleaseEvent(QMouseEvent* evt)
{
    if(evt->button() == Qt::RightButton)
    {
        QListWidgetItem* get = currentItem();

        if(get && (get->text() != ""))
        {
            emit right_btn_press();
            return;
        }
    }

    QListWidget :: mouseReleaseEvent(evt);
}
