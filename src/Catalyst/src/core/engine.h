#pragma once

#include "internal.h"

#include "../platform/Platform.h"

#include "graphic/renderer.h"
#include "graphic/surface.h"

namespace Catalyst
{

    class CATALYST_API Engine
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
         * @func createRenderer
         *
         * @brief Creates a renderer instance.
         *
         * It is up to you to control the lifetime of the renderer.
         * It is recommended that you store the renderer using a std::shared_ptr<IRenderer>,
         * then reset using the pointer to the renderer.
         *
         * \return IRenderer** - Renderer instance
         */
        static CatalystPtrRenderer createRenderer(CatalystPtrSurface surface, RendererInfo info);

        /**
         * .
         *
         * \return
         */
        static CatalystPtrSurface createSurface(SurfaceData);

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


}
