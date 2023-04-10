#pragma once

#include "engine.h"
#include "application.h"
#include "logging.h"


#ifndef CATALYST_HEADER_ENTRY
#define CATALYST_HEADER_ENTRY

namespace Catalyst
{

    extern CatalystResult CatalystLaunch();

    void testOpenGL();
    void testVulkan();

}

#if defined(CATALYST_MAIN) || 0//<-- For writting purposes only!

int main(int argc, char** argv, char** envp)
{

    Catalyst::testOpenGL();

    return 0;

    Catalyst::Engine::initalizeEngine(argc, argv, envp);

START:

    Catalyst::CatalystResult result = Catalyst::CatalystLaunch();

    Catalyst::CatalystApplicationClose();

    if (result == Catalyst::CatalystResult::IApplication_Recreation_Request)
        goto START;

    return (int)result;
}
#endif
#endif //CATALYST_HEADER_ENTRY

/**
 * CATALYST_LAUNCH - Launches the application.
 * 
 * @param APPLICATION - IApplication class to launch
 */
#define CATALYST_LAUNCH(APPLICATION)                           \
                                                               \
Catalyst::CatalystResult Catalyst::CatalystLaunch()            \
{                                                              \
    return Catalyst::CatalystApplicationLaunch<APPLICATION>(); \
}