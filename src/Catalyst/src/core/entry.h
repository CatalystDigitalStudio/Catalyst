#pragma once

#include "application.h"

using namespace std::chrono_literals;

#define CATALYST_LAUNCH(APPLICATION) Catalyst::LaunchApplication<APPLICATION>();

namespace Catalyst
{

    extern CatalystResult CatalystLaunch();

}

#if defined(CATALYST_MAIN) || 1//<-- For writting purposes only!
int main(int* argc, char* argv, char* envp)
{

    Catalyst::CatalystResult result = Catalyst::CatalystLaunch();

    return (int)result;
}
#endif