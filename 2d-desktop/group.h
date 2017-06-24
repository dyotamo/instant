#ifndef GROUP_H
#define GROUP_H

#include "chat.h"
#include "host.h"

#include "tabwidget.h"

class group : public chat
{
public:
    group(tabwidget *, QList<host>);

public slots:
    void onSend();

private:
    QList<host> m_hosts;
};

#endif // GROUP_H
