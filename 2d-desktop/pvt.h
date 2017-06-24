#ifndef PTV_H
#define PTV_H

#include "chat.h"
#include "host.h"
#include "tabwidget.h"

#include <tuple>

#include "cwindow.h"

using BufferTuple = std :: tuple<QString, QDateTime, QString>;

class pvt : public chat
{
    Q_OBJECT
public:
    pvt(tabwidget*, host, host, QList<BufferTuple>&);

protected slots:
    void onSend();

private:
    host me;
    host m_host;

    QList<BufferTuple>* msgs;

    friend class cwindow;
};

#endif // PTV_H
