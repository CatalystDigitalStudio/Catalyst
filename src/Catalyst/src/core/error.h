#pragma once

#include "internal.h"

#define CATALYST_ERROR2(Message, Reason) { Level::Error, Message, Reason, __FUNCTION__ }
#define CATALYST_ERROR(Reason) CATALYST_ERROR2(#Reason, Reason)

namespace Catalyst
{

    enum class Level
    {
        Warning,
        Error
    };

    struct CatalystError
    {
        Level level = Level::Error;
        const char* message = 0;
        CatalystResult reason = CatalystResult::Unknown;
        const char* function = 0;
    };

    typedef void (*CatalystErrorHandler)(CatalystError&&);

    /**
         * Set a custom error handler.
         *
         * @param handler - custom handler function (Cannot be a class function, namespace is premitted)
         */
    static void setErrorHandler(CatalystErrorHandler handler);

    /**
     * Sets a new error message to be handled.
     *
     * @param error - error information to pass on
     */
    static void raiseError(CatalystError&& error);

    /**
     * Gets the last raised error.
     */
    CATALYST_LOGIC_DISCARD static CatalystError getLastError();
}
