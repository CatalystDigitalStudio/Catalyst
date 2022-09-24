#include "phc.h"
#include "internal.h"

#include <future>

namespace Catalyst
{

    static void catalyst_s_DefaultHandler(CatalystError&& error)
    {
        switch (error.level)
        {
        case Level::Warning:
            CATALYST_CORE_WARN("[{0}] {1}", error.function, error.message); break;
        case Level::Error:
            CATALYST_CORE_ERROR("[{0}] {1}", error.function, error.message); break;
        default:
            CATALYST_CORE_ERROR("UNKNOWN ERROR LEVEL! [Error : {0}]", error.message); break;
        }
    }

    static std::atomic<CatalystError> catalyst_s_EngineError;
    static std::atomic<CatalystErrorHandler> catalyst_s_EngineErrorHandler = std::atomic<CatalystErrorHandler>(catalyst_s_DefaultHandler);

    enum class CatalystArguments
    {
        PROFILE_LOCATION,
        LOG_LOCATION
    };
    const char* catalystArguments[] =
    {
        "PROFILE_LOCATION",
        "LOG_LOCATION"
    };

    static std::vector<std::string> processArgument(const std::string& arguments)
    {
        std::vector<std::string> result;

        size_t start = arguments.find('{');
        size_t end   = arguments.find('}');

        if (start == arguments.npos || end == arguments.npos)
        {
            raiseEngineError({ Level::Error, "CatalystResult::Commandline_Argument_Malformed", CatalystResult::Commandline_Argument_Malformed, __FUNCTION__ });
        }

        ++start;

        std::string buffer = "";
        size_t last = start, next = start;

        
        while ((next = arguments.find(',', last)) != arguments.npos)
        {
            buffer = arguments.substr(last, next - last);
            result.emplace_back(buffer);
            last = next+1;
        }

        buffer = arguments.substr(last, end - last);
        result.emplace_back(buffer);

        return result;
    }

    void CatalystSetErrorHandler(CatalystErrorHandler handler)
    {
        catalyst_s_EngineErrorHandler.store(handler);
    }

    void CatalystInitalizeEngine(int argc, char** argv, char** envp)
    {
        Catalyst::Logger::initalizeDefault();

        std::string profile_location = "log/profile_logger.log";
        std::vector<std::string> logger_locations = { "log/default_logger.log" };

        for (int i = 0; i < argc; i++)
        {
            std::string arg = argv[i];

            if (arg.find(catalystArguments[(int)CatalystArguments::PROFILE_LOCATION]) != arg.npos)
            {
                profile_location = processArgument(arg)[0];
            }
            else if (arg.find(catalystArguments[(int)CatalystArguments::LOG_LOCATION]) != arg.npos)
            {
                logger_locations = processArgument(arg);
            }
            else
            {
                raiseEngineError({ Level::Warning, "CatalystResult::Commandline_Argument_Unknown", CatalystResult::Commandline_Argument_Unknown , __FUNCTION__ });
            }
        }

        for (auto& location : logger_locations)
        {
            Catalyst::Logger::addFile("catalyst_logger", location.c_str());
            Catalyst::Logger::addCoreFile("catalyst_core_logger", location.c_str());
        }

        Catalyst::Profile::initalize(profile_location.c_str());

    }
    void raiseEngineError(CatalystError&& error)
    {
        auto future = std::async(std::launch::async, catalyst_s_EngineErrorHandler.load(), error);

        catalyst_s_EngineError.store(error);
    }
    CatalystError getLastEgineError()
    {
        return catalyst_s_EngineError.load();
    }
}