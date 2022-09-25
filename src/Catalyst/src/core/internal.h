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

/**
 * catalyst_assert - asserts that X is true if not then it performs ACTION.
 * 
 * @param X - Expression to evaluate
 * @param ACTION - Action to take if X is not true 
 */
#define CATALYST_ASSERT(X, ACTION) if(!(X)) { ACTION; }

#ifdef CATALYST_DEBUG
/**
 * Same as catalyst_assert, but is only included if building in debug.
 */
#define CATALYST_DEBUG_ASSERT(X, ACTION) CATALYST_ASSERT(X, ACTION)

#else
 /**
  * Currently not building in debug
  */
#define CATALYST_DEBUG_ASSERT(X, ACTION)

#endif

#include "profiling.h"

#ifdef CATALYST_ENABLE_PROFILING
#define CATALYST_PROFILE_FUNCTION(RESULT) Catalyst::ProfileFunction catalyst_pFunction(__FUNCTION__, RESULT)

#ifdef CATALYST_ENABLE_CORE_PROFILING
#define CATALYST_PROFILE_CORE_FUNCTION(RESULT) Catalyst::ProfileFunction catalyst_pFunction(__FUNCTION__, RESULT)
#else
#define CATALYST_PROFILE_CORE_FUNCTION(RESULT)
#endif
#else
#define CATALYST_PROFILE_FUNCTION(RESULT)
#define CATALYST_PROFILE_CORE_FUNCTION(RESULT)
#endif

///----------------------------------------------
/// CATALYST INTERNALS
///----------------------------------------------

#include <mutex>

#ifndef CATALYST_HEADER_INTERNAL
#define CATALYST_HEADER_INTERNAL


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

        Pointer_Is_Nullptr                 = 0b00000000'00000000'00000001'00000001,
        End_Is_Less_Than_Begin             = 0b00000000'00000000'00000001'00000010,
        Destination_Is_Smaller_Than_Source = 0b00000000'00000000'00000001'00000011,

        Commandline_Argument_Unknown    = 0b00000000'00000000'00000010'00000000,
        Commandline_Argument_Malformed  = 0b00000000'00000000'00000010'00000100,

        Json_Object_Missing_Bracket            = 0b00000000'00000000'00001000'00000000,
        Json_Vector_Missing_Bracket            = 0b00000000'00000000'00001000'00000001,
        Json_String_Missing_Quotation_Mark     = 0b00000000'00000000'00001000'00000010,
        Json_Key_Missing_Quotation_Mark        = 0b00000000'00000000'00001000'00000010,

        Utf8_Codepoint_Malformed             = 0b00000000'00000000'00000100'00000000,
        Utf8_Codepoint_Found_Unexpected_Byte = 0b00000000'00000000'00000100'00000001,

        IApplication_Unspecified        = 0b00000000'00000000'10000000'00000001,
        IApplication_Failed_Creation    = 0b00000000'00000000'10000000'00000010,
        IApplication_Recreation_Request = 0b00000000'00000000'10000000'00000100,
    };

    enum class Level
    {
        Warning,
        Error
    };

    typedef enum class CatalystArguments CatalystArguments;
    extern const char* catalystArguments[];

    struct CatalystError
    {
        Level level = Level::Error;
        const char* message = 0;
        CatalystResult reason = CatalystResult::Unknown;
        const char* function = 0;
    };

    struct IMultithreadable
    {
        std::mutex m_Mutex;
    };

    ///==========================================================================================
    ///  CATALYST ERRORS
    ///==========================================================================================
    
    typedef void (*CatalystErrorHandler)(CatalystError&&);

    /**
     * Set a custom error handler.
     * 
     * @param handler - custom handler function (Cannot be a class function, namespace is premitted)
     */
    extern void CatalystSetErrorHandler(CatalystErrorHandler handler);

    /**
     * Sets a new error message to be handled.
     *
     * @param error - error information to pass on
     */
    extern void raiseEngineError(CatalystError&& error);

    /**
     * Gets the last raised error.
     */
    CATALYST_LOGIC_DISCARD extern CatalystError getLastEgineError();

    /**
     * Initalizes the engine using the provided arguments from the command-line and OS envioment.
     * 
     * @param argc - number of arguments
     * @param argv - pointer to list of arguments
     * @param envp - pointer to list of envioment varables
     */
    extern void CatalystInitalizeEngine(int argc, char** argv, char** envp);

    extern void CatalystUpdate();

    extern size_t CatalystGetCycleIndex();

    extern size_t CatalystGetAllocations();

    extern size_t CatalystGetAllocationAmount();


    /**
     * Copy a set of data from a source buffer to a destination buffer.
     * 
     * \param dest_begin - destination buffer begin
     * \param dest_end   - destination buffer end
     * \param src_begin  - source buffer begin
     * \param src_end    - source buffer end
     */
    template<typename T>
    static constexpr void copy(const T* dest_begin, const T* dest_end, const T* src_begin, const T* src_end)
    {
        CATALYST_DEBUG_ASSERT(dest_begin && dest_end && src_begin && src_end, raiseEngineError({ Level::Error, "CatalystResult::Pointer_Is_Nullptr", CatalystResult::Pointer_Is_Nullptr , __FUNCTION__ }));

        const size_t s_size = src_end - src_begin;
        const size_t d_size = dest_end - dest_begin;

        CATALYST_DEBUG_ASSERT(dest_begin < dest_end, raiseEngineError({Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin, __FUNCTION__ }));
        CATALYST_DEBUG_ASSERT(src_begin < src_end, raiseEngineError({Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin , __FUNCTION__ }));
        CATALYST_DEBUG_ASSERT(d_size >= s_size, raiseEngineError({Level::Error, "CatalystResult::Destination_Is_Smaller_Than_Source", CatalystResult::Destination_Is_Smaller_Than_Source, __FUNCTION__ }));

        T* d_begin = (T*)dest_begin;

        while (src_begin != src_end)
        {
            *d_begin = *src_begin;
            ++d_begin;
            ++src_begin;
        }
    }

    /**
     * First the first occurance of the delimiter in a buffer.
     * 
     * \param begin - beginning of buffer to search
     * \param end - end of buffer to search
     * \param delimiter - find this
     * \return - pointer to location in buffer, or null if not found
     */
    template<typename T>
    CATALYST_LOGIC_DISCARD static constexpr const T* find_first_of(const T* begin, const T* end, T&& delimiter)
    {
        CATALYST_DEBUG_ASSERT(begin && end, raiseEngineError({ Level::Error, "CatalystResult::Pointer_Is_Nullptr", CatalystResult::Pointer_Is_Nullptr , __FUNCTION__ }));
        CATALYST_DEBUG_ASSERT(begin < end, raiseEngineError({ Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin , __FUNCTION__ }));

        while (begin != end)
        {
            if (*begin == delimiter)
                return begin;
            else
                ++begin;
        }

        return nullptr;
    }
    /**
     * First the first occurance of the delimiter buffer in a different buffer.
     * 
     * \param begin - beginning buffer to search
     * \param end - end of buffer to search
     * \param delimiter_begin - beginning of delimiter buffer
     * \param delimiter_end - end of delimiter buffer
     * \return pointer to start of location in search buffer, or null if not found
     */
    template<typename T>
    CATALYST_LOGIC_DISCARD static constexpr const T* find_first_of(const T* begin, const T* end, const T* delimiter_begin, const T* delimiter_end)
    {
        CATALYST_DEBUG_ASSERT(begin && end && delimiter_begin && delimiter_end, raiseEngineError({ Level::Error, "CatalystResult::Pointer_Is_Nullptr", CatalystResult::Pointer_Is_Nullptr , __FUNCTION__ }));

        CATALYST_DEBUG_ASSERT(begin < end, raiseEngineError({ Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin , __FUNCTION__ }));
        CATALYST_DEBUG_ASSERT(delimiter_begin < delimiter_end, raiseEngineError({ Level::Error, "CatalystResult::End_Is_Less_Than_Begin", CatalystResult::End_Is_Less_Than_Begin , __FUNCTION__ }));

        while (begin != end)
        {
            if (*begin == *delimiter_begin)
            {
                const T* b_index = begin;
                bool found = true;
                for (const T* i = delimiter_begin; i != (delimiter_end - 1); i++, begin++)
                {
                    if (*i != *begin)
                    {
                        found = false;
                         break;
                    }
                }
                if (found)
                {
                    return b_index;
                }
            }
            else
            {
                ++begin;
            }
        }

        return nullptr;
    }

}

#endif //CATALYEST_HEADER_INTERNAL