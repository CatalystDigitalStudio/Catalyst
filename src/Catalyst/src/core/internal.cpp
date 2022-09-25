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
    static std::atomic_size_t catalyst_s_TickIndex = 0;
    static std::atomic_size_t catalyst_s_Allocations = 0;
    static std::atomic_size_t catalyst_s_AllocationsSize = 0;

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

        Catalyst::Profiler::initalize(profile_location.c_str());

    }
    void CatalystUpdate()
    {
        catalyst_s_TickIndex.store(catalyst_s_TickIndex + 1);
    }
    size_t CatalystGetCycleIndex()
    {
        return catalyst_s_TickIndex.load();
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

    size_t CatalystGetAllocations()
    {
        return catalyst_s_Allocations.load();
    }
    size_t CatalystGetAllocationAmount()
    {
        return catalyst_s_AllocationsSize.load();
    }

    namespace internal
    {
        static void CatalystAddAllocation(size_t amount) noexcept
        {
            catalyst_s_Allocations.store(catalyst_s_Allocations + 1);
            catalyst_s_AllocationsSize.store(catalyst_s_AllocationsSize + amount);
        }
        static void CatalystRemoveAllocation(size_t amount) noexcept
        {
            catalyst_s_Allocations.store(catalyst_s_Allocations - 1);
            catalyst_s_AllocationsSize.store(catalyst_s_AllocationsSize - amount);
        }
    }
}

#if CATALYST_TRACK_ALLOCATIONS

CATALYST_LOGIC_DISCARD void* operator new(std::size_t size)
{
    CATALYST_ASSERT(size < ~size_t(), throw);

    void* ptr = malloc(size);

    if (!ptr)
        throw;

    Catalyst::internal::CatalystAddAllocation(size);

    return ptr;
}
CATALYST_LOGIC_DISCARD void* operator new[](std::size_t size)
{
    CATALYST_ASSERT(size < ~size_t(), throw);

    void* ptr = malloc(size);

    Catalyst::internal::CatalystAddAllocation(size);

    if (!ptr)
        throw;

    Catalyst::CatalystGetAllocations();

    return ptr;
}
void operator delete(void* ptr) noexcept
{
    Catalyst::internal::CatalystRemoveAllocation(_msize(ptr));
    free(ptr);
}
void operator delete[](void* ptr) noexcept
{
    Catalyst::internal::CatalystRemoveAllocation(_msize(ptr));
    free(ptr);
}
#endif