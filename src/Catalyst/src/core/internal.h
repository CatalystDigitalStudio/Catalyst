#pragma once

///==========================================================================================
///  CATALYST CPP VERSION DEFINITIONS AND LOGIC
///==========================================================================================

#define CATALYST_MAJOR 0
#define CATALYST_MINOR 0
#define CATALYST_PATCH 1

#define CATALYST_MACRO_STRING(value) #value
#define CATALYST_CREATE_VERSION(MAJOR, MINOR, PATCH) CATALYST_MACRO_STRING(MAJOR) "." CATALYST_MACRO_STRING(MINOR) "." CATALYST_MACRO_STRING(PATCH)
#define CATALYST_VERSION CATALYST_CREATE_VERSION(CATALYST_MAJOR, CATALYST_MINOR, CATALYST_PATCH)

#define CATALYST_CPP_VERSION_14 201402L
#define CATALYST_CPP_VERSION_17 201703L
#define CATALYST_CPP_VERSION_20 202002L

#ifdef _WIN32
#define CATALYST_CPP_VERSION _MSVC_LANG
#else
#define CATALYST_CPP_VERSION __cplusplus
#endif

#define CATALYST_IS_CPP_VERSION_GREATER(VERSION) (CATALYST_CPP_VERSION >= VERSION)

///==========================================================================================
///  CATALYST CPP HELPER DEFINITIONS
///==========================================================================================

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

#ifdef CATALYST_DLL
#ifdef _WIN32
#ifdef CATALYST_EXPORT
#define CATALYST_API _declspec(dllexport)
#else
#define CATALYST_API _declspec(dllimport)
#endif
#endif
#else
#define CATALYST_API
#endif
///----------------------------------------------
/// CATALYST INTERNALS
///----------------------------------------------

#include <mutex>
#include "utilities.h"
#include "managment/assets.h"

#ifndef CATALYST_HEADER_INTERNAL
#define CATALYST_HEADER_INTERNAL


namespace Catalyst
{

#if __cpp_char8_t
    using catalyst_char = char8_t;
#else
    using catalyst_char = char;
#endif

    enum class CATALYST_API CatalystResult
    {
        Unknown = -2,
        Error = -1,
        Success = 0,

        Pointer_Is_Nullptr                 = 0b00000000'00000000'00000001'00000001,
        End_Is_Less_Than_Begin             = 0b00000000'00000000'00000001'00000010,
        Destination_Is_Smaller_Than_Source = 0b00000000'00000000'00000001'00000011,
        Executing_Unreachable_Code         = 0b00000000'00000000'00000001'00000101,

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

    typedef void (*CatalystErrorHandler)(CatalystError&&);

    class IRenderer;
    class ISurface;
    class Platform;

    namespace internal
    {
        extern std::shared_ptr<ISurface> createSurface();
    }

    class Engine
    {

    public:

        ///==========================================================================================
        ///  Engine
        ///==========================================================================================

        /**
         * Initalizes the engine using the provided arguments from the command-line and OS envioment.
         *
         * @param argc - number of arguments
         * @param argv - pointer to list of arguments
         * @param envp - pointer to list of envioment varables
         */
        static void initalizeEngine(int argc, char** argv, char** envp);

        /**
         * Updates the engine tick and any other functions.
         */
        static void updateEngine();

        /**
         * Get the engine version as a string (Ex. "x.x.x").
         * 
         * @see https://semver.org/
         * 
         * \return string - version
         */
        static const char* const getVersion();

        /**
         * Get the major release version.
         * 
         * \return version major
         */
        static const unsigned char getMajorVersion();
        /**
         * Get the minor release version.
         *
         * \return version minor
         */
        static const unsigned char getMinorVersion();
        /**
         * Get the patch version.
         *
         * \return version patch
         */
        static const unsigned char getPatchVersion();

        ///==========================================================================================
        ///  Object creation and management
        ///==========================================================================================
        
        /**
         * .
         * 
         * \param type
         * \return 
         */
        static std::shared_ptr<IRenderer> launchRenderer(unsigned int type);

        /**
         * .
         * 
         * \param key
         * \param ...args
         * \return 
         */
        template<typename Value, typename Key, typename... Args>
        inline static auto requestAsset(Key key, Args&&... args)
        {
            return AssetManager<Value>::request(key, args...);
        }

        /**
         * .
         * 
         * \return 
         */
        static const std::shared_ptr<Platform> getPlatform();

        /**
         * .
         * 
         * \return 
         */
        static std::shared_ptr<ISurface> createSurface();

        ///==========================================================================================
        ///  Statistics and Profiling
        ///==========================================================================================
        /**
         * @function getTickCount().
         * 
         * @brief Returns the number of ticks since the start of the program
         * 
         * \return size_t - Ticks
         */
        static size_t getTickCount();

        /**
         * .
         * 
         * \return 
         */
        static size_t getAllocations();

        /**
         * .
         * 
         * \return 
         */
        static size_t getAllocationAmount();

#ifdef CATALYST_CORE
    private:
        /**
         * @function addAllocation()
         * 
         * @brief Increments the allocation counter
         * 
         * \param amount - size_t of allocation in bytes
         */
        static void addAllocation(size_t amount) noexcept;

        /**
         * @function removeAllocation()
         *
         * @brief Decrements the allocation counter
         *
         * \param amount - size_t of allocation in bytes
         */
        static void removeAllocation(size_t amount) noexcept;
#endif

    public:
        ///==========================================================================================
        ///  CATALYST ERRORS
        ///==========================================================================================

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

    private:
        static std::shared_ptr<Platform> catalyst_s_Platform;
        static std::atomic<CatalystError> catalyst_s_EngineError;
        static std::atomic<CatalystErrorHandler> catalyst_s_EngineErrorHandler;
        static std::atomic_size_t catalyst_s_TickIndex;
        static std::atomic_size_t catalyst_s_Allocations;
        static std::atomic_size_t catalyst_s_AllocationsSize;

    };


    /**
     * Copy a set of data from a source buffer to a destination buffer.
     * 
     * \param dest_begin - destination buffer begin
     * \param dest_end   - destination buffer end
     * \param src_begin  - source buffer begin
     * \param src_end    - source buffer end
     */
    template<typename T>
    static constexpr void copy(const T* dest_begin, const T* dest_end, const T* src_begin, const T* src_end);

    /**
     * First the first occurance of the delimiter in a buffer.
     * 
     * \param begin - beginning of buffer to search
     * \param end - end of buffer to search
     * \param delimiter - find this
     * \return - pointer to location in buffer, or null if not found
     */
    template<typename T>
    CATALYST_LOGIC_DISCARD static constexpr const T* find_first_of(const T* begin, const T* end, T&& delimiter);

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
    CATALYST_LOGIC_DISCARD static constexpr const T* find_first_of(const T* begin, const T* end, const T* delimiter_begin, const T* delimiter_end);

}

#include "internal.inl"

#endif //CATALYEST_HEADER_INTERNAL