#pragma once

#include "logging.h"
#include "internal.h"
#include "utilities.h"
#include "scene.h"

#include <mutex>
#include <atomic>

#ifndef CATALYST_HEADER_APPLICATION
#define CATALYST_HEADER_APPLICATION

namespace Catalyst
{

    class IApplication : public IMultithreadable
    {

    public:
        /**
         * Returns the current application instance.
         * 
         * \return IApplication*
         */
        CATALYST_LOGIC_DISCARD static std::shared_ptr<IApplication> get();

    protected:
        explicit IApplication(const char* process_name);

    public:
        ~IApplication();

        /**
         * Pure virtual, your main function.
         *
         */
        virtual void Run() = 0;

    public:
        /**
         * Should the application close.
         * 
         * \return bool
         */
        CATALYST_LOGIC_DISCARD const bool close() const;

        /**
         * Should the application close.
         *
         * \return bool
         */
        CATALYST_LOGIC_DISCARD const char* name() const;

        /**
         * Get the current scene.
         *
         * \return Scene pointer
         */
        CATALYST_LOGIC_DISCARD Scene* getScene();

        /**
         * Launch new scene.
         *
         * \return bool
         */
        template<typename S>
        void launchScene()
        {
            std::scoped_lock lock(m_Mutex);

            closeScene();

            m_Scene = std::make_unique<S>(this);
            m_Scene->onCreate();
        }
        void closeScene();

        /**
         * Set the application to close.
         */
        void close(const bool);


        /** Static variables and flags */
    private:
        static std::atomic<std::shared_ptr<IApplication>> s_Instance; /** <! s_Instance - Singleton instance of the application */
        static std::atomic_bool s_Reload; /** <! s_Reload - Flag the application to restart and close */
        static std::unique_ptr<Scene> m_Scene; /** <! s_Scene - The current scene>*/

        /** Friends */
    private:
        template<typename T, typename R>
        friend static CatalystResult CatalystApplicationLaunch();
        friend static void           CatalystApplicationClose();
        friend static void           CatalystApplicationReload();

    private:
        const char* m_Name;
        std::atomic_bool m_Close;
        
    };

    /**
     * Closes and restarts the application.
     * 
     * @brief IMPORTANT: For internal engine use only
     */
    static void CatalystApplicationReload()
    {
        IApplication::get()->m_Close.store(true);
        IApplication::s_Reload.store(true);
    }

    /**
     * Closes the application and cleans up.
     * 
     * @brief IMPORTANT: For internal engine use only
     */
    static void CatalystApplicationClose()
    {
        if (!IApplication::get())
            return;

        IApplication::get()->m_Close.store(true);

        IApplication::s_Instance.load().reset();
    }

    /**
     * Launches a new application under a new thread.
     * 
     * @brief IMPORTANT: For internal engine use only
     */
    template<typename T, typename R = std::enable_if_t<std::is_base_of_v<IApplication, T>, T>>
    CATALYST_LOGIC_DISCARD static CatalystResult CatalystApplicationLaunch()
    {
        CATALYST_PROFILE_CORE_FUNCTION(nullptr);

        CatalystApplicationClose();

        IApplication::s_Instance.store(std::make_shared<R>());

        if(!IApplication::get())
            return CatalystResult::IApplication_Failed_Creation;

        IApplication::get()->Run();

        if (IApplication::get()->s_Reload)
            return CatalystResult::IApplication_Recreation_Request;
        else
            return CatalystResult::Success;
    }
}

#endif //CATALYST_HEADER_APPLICATION