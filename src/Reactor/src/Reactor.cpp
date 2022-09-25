#define CATALYST_MAIN

#define CATALYST_ENABLE_PROFILING
#define CATALYST_ENABLE_CORE_PROFILING

#include "Reactor.h"

#include <future>
#include <fstream>
#include <streambuf>

CATALYST_LAUNCH(Reactor);

void ReactorErrorHandler(Catalyst::CatalystError&& error)
{
    CATALYST_LOG_ERROR(error.message);
}

void disision()
{
    CATALYST_LOG_INFO("Press any key to exit...");
    auto result = getc(stdin);
    Catalyst::IApplication::Get()->Close(true);
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
    CATALYST_PROFILE_FUNCTION(nullptr);

    auto f = std::async(disision);

    while (!Close())
    {
        Catalyst::CatalystUpdate();
    }

    f.wait();

    CATALYST_LOG_INFO("Allocation : {0}", Catalyst::CatalystGetAllocationAmount());
}