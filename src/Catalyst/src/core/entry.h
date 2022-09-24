#pragma once

#include "internal.h"
#include "application.h"
#include "logging.h"

#ifndef CATALYST_HEADER_ENTRY
#define CATALYST_HEADER_ENTRY

namespace Catalyst
{

    extern CatalystResult CatalystLaunch();

}

#if defined(CATALYST_MAIN) || 0//<-- For writting purposes only!

int main(int argc, char** argv, char** envp)
{

    Catalyst::CatalystInitalizeEngine(argc, argv, envp);

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