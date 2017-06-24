#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>
#include <QKeyEvent>

class listwidget : public QListWidget
{
    Q_OBJECT
public:
    explicit listwidget(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent*);
    void mouseReleaseEvent(QMouseEvent*);

signals:
    void return_pressed(QListWidgetItem*);
    void right_btn_press();

public slots:

};

#endif // LISTWIDGET_H
