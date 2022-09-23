#pragma once

///----------------------------------------------
/// CATALYST CPP VERSION DEFINITIONS AND LOGIC
///----------------------------------------------
#define CATALYST_CPP_VERSION_14 201402L
#define CATALYST_CPP_VERSION_17 201703L
#define CATALYST_CPP_VERSION_20 202002L

#ifdef _WIN32
#define CATALYST_CPP_VERSION _MSVC_LANG
#else
#define CATALYST_CPP_VERSION __cplusplus
#endif

#define CATALYST_IS_CPP_VERSION_GREATER(VERSION) (CATALYST_CPP_VERSION >= VERSION)

///----------------------------------------------
/// CATALYST CPP HELPER DEFINITIONS
///----------------------------------------------

#if CATALYST_IS_CPP_VERSION_GREATER( CATALYST_CPP_VERSION_20 )
    #define CATALYST_LOGIC_DISCARD [[nodiscard]]
#elif CATALYST_IS_CPP_VERSION_GREATER( CATALYST_CPP_VERSION_17 )
    #define CATALYST_LOGIC_DISCARD [[nodiscard]]
#else
    #error C++ versions less than 17 not supported
#endif

///----------------------------------------------
/// CATALYST INTERNALS
///----------------------------------------------

#include <mutex>

#ifndef CATALYST_INTERNAL_HEADER
#define CATALYST_INTERNAL_HEADER

namespace Catalyst
{

#if __cpp_char8_t
    using catalyst_char = char8_t;
#else
    using catalyst_char = char;
#endif

    enum class CatalystResult
    {
        Unknown = -2,
        Error = -1,
        Success = 0,

        IApplication_Unspecified        = 0x0100'0001,
        IApplication_Failed_Creation    = 0x0100'0002,
        IApplication_Recreation_Request = 0x0100'0100,
    };

    struct CatalystError
    {
        const char* message;
        CatalystResult reason;
    };


    struct IMultithreadable
    {
        std::mutex m_Mutex;
    };

}

#endif //CATALYEST_INTERNAL_HEADER