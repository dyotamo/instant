#include "group.h"

group::group(tabwidget* parent, QList<host> frnds) : chat(parent, "group"), m_hosts(frnds)
{
}

void group :: onSend()
{

}
