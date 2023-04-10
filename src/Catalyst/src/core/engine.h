#pragma once

#include "internal.h"

#include "../platform/Platform.h"

#include "graphic/renderer.h"
#include "graphic/surface.h"

namespace Catalyst
{

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
        static const std::string getVersion();

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

#ifdef CATALYST_CORE
        /**
         * .
         *
         * \return
         */
        static const std::shared_ptr<Platform> getPlatform();

#endif
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

        

    private:
        static std::shared_ptr<Platform> catalyst_s_Platform;
        static std::atomic_size_t catalyst_s_TickIndex;
        static std::atomic_size_t catalyst_s_Allocations;
        static std::atomic_size_t catalyst_s_AllocationsSize;

    };


}
