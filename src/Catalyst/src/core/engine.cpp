#include "pch.h"
#include "engine.h"

#include "events.h"

namespace Catalyst
{

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
        size_t end = arguments.find('}');

        if (start == arguments.npos || end == arguments.npos)
        {
            raiseError(CATALYST_ERROR(CatalystResult::Commandline_Argument_Malformed));
        }

        ++start;

        std::string buffer = "";
        size_t last = start, next = start;


        while ((next = arguments.find(',', last)) != arguments.npos)
        {
            buffer = arguments.substr(last, next - last);
            result.emplace_back(buffer);
            last = next + 1;
        }

        buffer = arguments.substr(last, end - last);
        result.emplace_back(buffer);

        return result;
    }


    std::shared_ptr<Platform>                                            Engine::catalyst_s_Platform = Catalyst::initalizePlatform();
    
    std::atomic_size_t                                                   Engine::catalyst_s_TickIndex = 0;
    std::atomic_size_t                                                   Engine::catalyst_s_Allocations = 0;
    std::atomic_size_t                                                   Engine::catalyst_s_AllocationsSize = 0;

    ///==========================================================================================
    ///  Engine
    ///==========================================================================================

    void Engine::initalizeEngine(int argc, char** argv, char** envp)
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
                raiseError({ Level::Warning, "CatalystResult::Commandline_Argument_Unknown", CatalystResult::Commandline_Argument_Unknown , __FUNCTION__ });
            }
        }

        for (auto& location : logger_locations)
        {
            Catalyst::Logger::addFile("catalyst_logger", location.c_str());
            Catalyst::Logger::addCoreFile("catalyst_core_logger", location.c_str());
        }

        Catalyst::Profiler::initalize(profile_location.c_str());

    }

    void Engine::updateEngine()
    {
        catalyst_s_TickIndex.store(catalyst_s_TickIndex + 1);
        ListenerManager::update();
    }

    const std::string const Engine::getVersion()
    {
        return std::string(CATALYST_VERSION);
    }
    const unsigned char Engine::getMajorVersion()
    {
        return CATALYST_MAJOR;
    }
    const unsigned char Engine::getMinorVersion()
    {
        return CATALYST_MINOR;
    }
    const unsigned char Engine::getPatchVersion()
    {
        return CATALYST_PATCH;
    }

    ///==========================================================================================
    ///  Object creation
    ///==========================================================================================
    
    const std::shared_ptr<Platform> Engine::getPlatform()
    {
        return catalyst_s_Platform;
    }

    ///==========================================================================================
    ///  Statistics and Profiling
    ///==========================================================================================

    size_t Engine::getTickCount()
    {
        return catalyst_s_TickIndex.load();
    }


    size_t Engine::getAllocations()
    {
        return catalyst_s_Allocations.load();
    }
    size_t Engine::getAllocationAmount()
    {
        return catalyst_s_AllocationsSize.load();
    }
    void Engine::addAllocation(size_t amount) noexcept
    {
        catalyst_s_Allocations.store(catalyst_s_Allocations + 1);
        catalyst_s_AllocationsSize.store(catalyst_s_AllocationsSize + amount);
    }
    void Engine::removeAllocation(size_t amount) noexcept
    {
        catalyst_s_Allocations.store(catalyst_s_Allocations - 1);
        catalyst_s_AllocationsSize.store(catalyst_s_AllocationsSize - amount);
    }

    
}
