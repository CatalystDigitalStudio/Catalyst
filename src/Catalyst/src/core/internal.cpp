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

    std::atomic<CatalystError>                                           Engine::catalyst_s_EngineError;
    std::atomic<CatalystErrorHandler>                                    Engine::catalyst_s_EngineErrorHandler = std::atomic<CatalystErrorHandler>(catalyst_s_DefaultHandler);
    std::atomic_size_t                                                   Engine::catalyst_s_TickIndex = 0;
    std::atomic_size_t                                                   Engine::catalyst_s_Allocations = 0;
    std::atomic_size_t                                                   Engine::catalyst_s_AllocationsSize = 0;

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
            Engine::raiseError({ Level::Error, "CatalystResult::Commandline_Argument_Malformed", CatalystResult::Commandline_Argument_Malformed, __FUNCTION__ });
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
    }

    const char* const Engine::getVersion()
    {
        return CATALYST_VERSION;
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
    

    IRenderer* Engine::launchRenderer(unsigned int type)
    {
        return nullptr;
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

    ///==========================================================================================
    ///  CATALYST ERRORS
    ///==========================================================================================
    void Engine::setErrorHandler(CatalystErrorHandler handler)
    {
        catalyst_s_EngineErrorHandler.store(handler);
    }
    void Engine::raiseError(CatalystError&& error)
    {
        auto future = std::async(std::launch::async, catalyst_s_EngineErrorHandler.load(), error);

        catalyst_s_EngineError.store(error);
    }
    CatalystError Engine::getLastError()
    {
        return catalyst_s_EngineError.load();
    }
}

#if CATALYST_TRACK_ALLOCATIONS

CATALYST_LOGIC_DISCARD void* operator new(std::size_t size)
{
    CATALYST_ASSERT(size < ~size_t(), throw);

    void* ptr = malloc(size);

    if (!ptr)
        throw;

    Catalyst::Engine::addAllocation(size);

    return ptr;
}
CATALYST_LOGIC_DISCARD void* operator new[](std::size_t size)
{
    CATALYST_ASSERT(size < ~size_t(), throw);

    void* ptr = malloc(size);

    Catalyst::Engine::addAllocation(size);

    if (!ptr)
        throw;

    Catalyst::CatalystGetAllocations();

    return ptr;
}
void operator delete(void* ptr) noexcept
{
    Catalyst::Engine::removeAllocation(_msize(ptr));
    free(ptr);
}
void operator delete[](void* ptr) noexcept
{
    Catalyst::Engine::removeAllocation(_msize(ptr));
    free(ptr);
}
#endif