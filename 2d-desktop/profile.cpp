#include "profile.h"

profile::profile(abstracttab* parent) : abstracttab(parent, "profile")
{
    initComps();
}

void profile ::initComps()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;

    mainLayout->addStretch();

    QHBoxLayout* innerLayout = new QHBoxLayout;

    QVBoxLayout* infos = new QVBoxLayout;

    innerLayout->addLayout(infos);

    infos->addWidget(lblID = new QLabel("<font color=red>ID:</font> carlee4ever"));
    infos->addWidget(lblName = new QLabel("<font color=red>Name:</font> Dássone José Yotamo"));
    infos->addWidget(lblDOB = new QLabel("<font color=red>Data de nascimento:</font> 05-09-1993"));
    infos->addWidget(lblSex = new QLabel("<font color=red>Sexo:</font> Masculino"));
    infos->addWidget(lblFriends = new QLabel("<font color=red>Número de amigos:</font> 2872"));
    infos->addWidget(lblDOI = new QLabel("<font color=red>Registado em:</font> 25-03-2014"));

    mainLayout->addLayout(innerLayout);
    mainLayout->addWidget(lblAbout = new QLabel("<font color=green>\"Qt Creator 3.0.0\n"
                                                "Based on Qt 5.2.0 (GCC 4.6.1, 32 bit)\n"
                                                "Built on Dec 10 2013 at 11:47:52\n</font>"));
    lblAbout->setAlignment(Qt::AlignCenter);

    mainLayout->addStretch();

    setLayout(mainLayout);
}
