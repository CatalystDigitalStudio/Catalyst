#pragma once

#include "application.h"
#include "logging.h"

#define CATALYST_LAUNCH(APPLICATION) Catalyst::LaunchApplication<APPLICATION>();

#ifndef CATALYST_ENTRY_HEADER
#define CATALYST_ENTRY_HEADER

namespace Catalyst
{

    extern CatalystResult CatalystLaunch();

}

#if defined(CATALYST_MAIN) || 1//<-- For writting purposes only!
int main(int* argc, char* argv, char* envp)
{

    Catalyst::Logger::initalizeDefault();
    Catalyst::Logger::addFile("catalyst_core_logger", "log/core_logger.log");

START:
    CATALYST_INFO("*************************\tCATALYST ENGINE STARTING\t*************************\r\n");

    Catalyst::CatalystResult result = Catalyst::CatalystLaunch();

    Catalyst::CloseApplication();

    if (result == Catalyst::CatalystResult::IApplication_Recreation_Request)
        goto START;

    CATALYST_INFO("*************************\tCATALYST ENGINE STOPING\t*************************\r\n");

    return (int)result;
}
#endif

#endif //CATALYST_ENTRY_HEADER
