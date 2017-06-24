#ifndef PROFILE_H
#define PROFILE_H

#include "abstracttab.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class profile : public abstracttab
{
public:
    profile(abstracttab*);

    virtual void initComps();

private:

private:
    QLabel* lblID;
    QLabel* lblName;
    QLabel* lblDOB;
    QLabel* lblSex;
    QLabel* lblFriends;
    QLabel* lblDOI;

    QLabel* lblAbout;
};

#endif // PROFILE_H
