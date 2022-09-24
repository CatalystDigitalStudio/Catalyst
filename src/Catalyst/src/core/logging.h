#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/logger.h"

#ifndef CATALYST_HEADER_LOGGING
#define CATALYST_HEADER_LOGGING

namespace Catalyst
{
    

    class Logger
    {
    public:
        using logger = std::shared_ptr<spdlog::logger>;

    private:
        static logger catalyst_s_logger;
        static logger catalyst_s_core_logger;

    public:
        static spdlog::sink_ptr addConsole();
        static spdlog::sink_ptr addFile(const char* name, const char* location);

        static void removeSink(spdlog::sink_ptr);

        static spdlog::sink_ptr addCoreConsole();
        static spdlog::sink_ptr addCoreFile(const char* name, const char* location);

        static void removeCoreSink(spdlog::sink_ptr);

        static void initalizeDefault();
        static void cleanup();

        CATALYST_LOGIC_DISCARD static logger getLogger();
#ifdef CATALYST_CORE
        CATALYST_LOGIC_DISCARD static logger getCoreLogger();
#endif

    };

}

#define CATALYST_LOG_TRACE(...)    Catalyst::Logger::getLogger()->trace(__VA_ARGS__)
#define CATALYST_LOG_INFO(...)     Catalyst::Logger::getLogger()->info(__VA_ARGS__)
#define CATALYST_LOG_WARN(...)     Catalyst::Logger::getLogger()->warn(__VA_ARGS__)
#define CATALYST_LOG_ERROR(...)    Catalyst::Logger::getLogger()->error(__VA_ARGS__)
#define CATALYST_LOG_CRITICAL(...) Catalyst::Logger::getLogger()->critcal(__VA_ARGS__)

#ifdef CATALYST_CORE
#define CATALYST_CORE_TRACE(...)    Catalyst::Logger::getCoreLogger()->trace(__VA_ARGS__)
#define CATALYST_CORE_INFO(...)     Catalyst::Logger::getCoreLogger()->info(__VA_ARGS__)
#define CATALYST_CORE_WARN(...)     Catalyst::Logger::getCoreLogger()->warn(__VA_ARGS__)
#define CATALYST_CORE_ERROR(...)    Catalyst::Logger::getCoreLogger()->error(__VA_ARGS__)
#define CATALYST_CORE_CRITICAL(...) Catalyst::Logger::getCoreLogger()->critcal(__VA_ARGS__)
#endif

#endif //CATALYEST_HEADER_LOGGING