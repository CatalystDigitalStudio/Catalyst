#include "pch.h"

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

    static std::atomic<CatalystError>        catalyst_s_EngineError;
    static std::atomic<CatalystErrorHandler> catalyst_s_EngineErrorHandler = std::atomic<CatalystErrorHandler>(catalyst_s_DefaultHandler);

    void setErrorHandler(CatalystErrorHandler handler)
    {
        catalyst_s_EngineErrorHandler.store(handler);
    }
    void raiseError(CatalystError&& error)
    {
        auto future = std::async(std::launch::async, catalyst_s_EngineErrorHandler.load(), error);

        catalyst_s_EngineError.store(error);
    }
    CatalystError getLastError()
    {
        return catalyst_s_EngineError.load();
    }

}
