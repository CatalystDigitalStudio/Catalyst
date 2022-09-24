#define CATALYST_MAIN

#define CATALYST_ENABLE_PROFILING
#define CATALYST_ENABLE_CORE_PROFILING

#include "Reactor.h"

CATALYST_LAUNCH(Reactor);

void ReactorErrorHandler(Catalyst::CatalystError&& error)
{
    CATALYST_LOG_ERROR(error.message);
}

Reactor::Reactor()
    : Catalyst::IApplication("Reactor")
{
    Catalyst::CatalystSetErrorHandler(ReactorErrorHandler);
}
Reactor::~Reactor()
{
}
void Reactor::Run()
{

    const char buffer[] = "Hello world!";
    const char delim[] = "or";

    {
        CATALYST_PROFILE_FUNCTION(nullptr);
        auto result = Catalyst::find_first_of(&buffer[0], &buffer[sizeof(buffer)], &delim[0], &delim[sizeof(delim)]);
    }


    while (!Close())
    {
        char c = getc(stdin);

        switch (c)
        {
        case 'r':
            Catalyst::CatalystApplicationReload();
        case 'x':
            return;
        default:
            ;
        }
    }
}