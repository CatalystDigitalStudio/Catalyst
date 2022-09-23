#define CATALYST_MAIN

#include "Reactor.h"


Reactor::Reactor()
    : Catalyst::IApplication("Reactor")
{

}
Reactor::~Reactor()
{

}
void Reactor::Run()
{

    while (!Close())
    {
        char c = getc(stdin);

        switch (c)
        {
        case 'r':
            Catalyst::ReloadApplication();
        case 'x':
            return;
        default:
            ;
        }
    }
}



Catalyst::CatalystResult Catalyst::CatalystLaunch()
{
    return CATALYST_LAUNCH(Reactor);
}
