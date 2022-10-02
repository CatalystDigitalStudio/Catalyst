#define CATALYST_MAIN

#define CATALYST_ENABLE_PROFILING
#define CATALYST_ENABLE_CORE_PROFILING

#include "Reactor.h"

#include <future>
#include <fstream>
#include <streambuf>

using namespace std::chrono_literals;

CATALYST_LAUNCH(Reactor);

void ReactorErrorHandler(Catalyst::CatalystError&& error)
{
    CATALYST_LOG_ERROR(error.message);
}

Reactor::Reactor()
    : Catalyst::IApplication("Reactor")
{
    Catalyst::Engine::setErrorHandler(ReactorErrorHandler);
}
Reactor::~Reactor()
{
}
void Reactor::Run()
{
    CATALYST_PROFILE_FUNCTION(nullptr);

    auto surface = Catalyst::Engine::createSurface();
    surface->create();

    auto renderer = Catalyst::Engine::launchRenderer(0);
    renderer->initalize(surface);

    while (!close())
    {
        surface->update();
        Catalyst::Engine::updateEngine();

        std::this_thread::sleep_for(10ms);
    }

    renderer->cleanup();
    surface->destroy();

    CATALYST_LOG_INFO("Allocation : {0}", Catalyst::Engine::getAllocationAmount());
}

bool Reactor::onEvent(TestEvent&& event)
{
    event.i;
    Catalyst::IApplication::get()->close(true);
    return true;
}
