#pragma once

#include "spdlog/spdlog.h"

#ifndef CATALYST_LOGGING_HEADER
#define CATALYST_LOGGING_HEADER

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


        template<typename... Args>
        static inline void trace(const char* msg, Args&&... args)
        {
            catalyst_s_logger->trace(msg, args...);
            catalyst_s_logger->flush();
        }

        template<typename... Args>
        static inline void info(const char* msg, Args&&... args)
        {
            catalyst_s_logger->info(msg, args...);
            catalyst_s_logger->flush();
        }

        template<typename... Args>
        static inline void warn(const char* msg, Args&&... args)
        {
            catalyst_s_logger->warn(msg, args...);
            catalyst_s_logger->flush();
        }
        template<typename... Args>
        static inline void error(const char* msg, Args&&... args)
        {
            catalyst_s_logger->error(msg, args...);
            catalyst_s_logger->flush();
        }
        template<typename... Args>
        static inline void critical(const char* msg, Args&&... args)
        {
            catalyst_s_logger->critical(msg, args...);
            catalyst_s_logger->flush();
        }


        template<typename... Args>
        static inline void core_trace(const char* msg, Args&&... args)
        {
            catalyst_s_core_logger->trace(msg, args...);
            catalyst_s_core_logger->flush();
        }

        template<typename... Args>
        static inline void core_info(const char* msg, Args&&... args)
        {
            catalyst_s_core_logger->info(msg, args...);
            catalyst_s_core_logger->flush();
        }

        template<typename... Args>
        static inline void core_warn(const char* msg, Args&&... args)
        {
            catalyst_s_core_logger->warn(msg, args...);
            catalyst_s_core_logger->flush();
        }
        template<typename... Args>
        static inline void core_error(const char* msg, Args&&... args)
        {
            catalyst_s_core_logger->error(msg, args...);
            catalyst_s_core_logger->flush();
        }
        template<typename... Args>
        static inline void core_critical(const char* msg, Args&&... args)
        {
            catalyst_s_core_logger->critical(msg, args...);
            catalyst_s_core_logger->flush();
        }

    };

}

#define CATALYST_TRACE(...)    Catalyst::Logger::trace(__VA_ARGS__)
#define CATALYST_INFO(...)     Catalyst::Logger::info(__VA_ARGS__)
#define CATALYST_WARN(...)     Catalyst::Logger::warn(__VA_ARGS__)
#define CATALYST_ERROR(...)    Catalyst::Logger::error(__VA_ARGS__)
#define CATALYST_CRITICAL(...) Catalyst::Logger::critcal(__VA_ARGS__)

#define CATALYST_CORE_TRACE(...)    Catalyst::Logger:core_trace(__VA_ARGS__)
#define CATALYST_CORE_INFO(...)     Catalyst::Logger:core_info(__VA_ARGS__)
#define CATALYST_CORE_WARN(...)     Catalyst::Logger:core_warn(__VA_ARGS__)
#define CATALYST_CORE_ERROR(...)    Catalyst::Logger:core_error(__VA_ARGS__)
#define CATALYST_CORE_CRITICAL(...) Catalyst::Logger:core_critcal(__VA_ARGS__)

#endif //CATALYEST_LOGGING_HEADER