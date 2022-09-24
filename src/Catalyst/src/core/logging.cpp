#include "phc.h"
#include "logging.h"

#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Catalyst
{

    Logger::logger Logger::catalyst_s_logger;
    Logger::logger Logger::catalyst_s_core_logger;

    spdlog::sink_ptr Logger::addConsole()
    {
        return catalyst_s_logger->sinks().emplace_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
    }
    spdlog::sink_ptr Logger::addFile(const char* name, const char* location)
    {
        return catalyst_s_logger->sinks().emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(name, location));
    }
    void Logger::removeSink(spdlog::sink_ptr ptr)
    {
        auto sinks = catalyst_s_logger->sinks();
        sinks.erase(std::remove(sinks.begin(), sinks.end(), ptr));
    }

    spdlog::sink_ptr Logger::addCoreConsole()
    {
        return catalyst_s_core_logger->sinks().emplace_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
    }
    spdlog::sink_ptr Logger::addCoreFile(const char* name, const char* location)
    {
        return catalyst_s_core_logger->sinks().emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(name, location));
    }
    void Logger::removeCoreSink(spdlog::sink_ptr ptr)
    {
        auto sinks = catalyst_s_core_logger->sinks();
        sinks.erase(std::remove(sinks.begin(), sinks.end(), ptr));
    }


    void Logger::initalizeDefault()
    {
        catalyst_s_logger = std::make_shared<spdlog::logger>("catalyst_logger");
        catalyst_s_core_logger = std::make_shared<spdlog::logger>("catalyst_core_logger");

        spdlog::register_logger(catalyst_s_logger);
        spdlog::register_logger(catalyst_s_core_logger);

#if CATALYST_DEBUG
        Catalyst::Logger::addConsole();
        Catalyst::Logger::addCoreConsole();
#endif
    }
    void Logger::cleanup()
    {
        //TODO
    }
    CATALYST_LOGIC_DISCARD Logger::logger Logger::getLogger()
    {
        return catalyst_s_logger;
    }
    CATALYST_LOGIC_DISCARD Logger::logger Logger::getCoreLogger()
    {
        return catalyst_s_core_logger;
    }
}