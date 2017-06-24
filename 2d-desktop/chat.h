#ifndef CHAT_H
#define CHAT_H

#include "abstracttab.h"
#include "tabwidget.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

class chat : public abstracttab
{
    Q_OBJECT
public:
    chat(tabwidget*, QString);

protected:

private slots:
    void onedittext(QString);

protected:
    virtual void onSend() = 0;

protected:
    QTextEdit* txts;
    QLineEdit* msg;
    QPushButton* btnSend;

private:
    void initComps();
};

#endif // CHAT_H
