#pragma once

#include "internal.h"
#include "type_traits.h"

#include <mutex>

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
        CATALYST_LOGIC_DISCARD static std::shared_ptr<IApplication> Get();

    public:
        ~IApplication();

        /**
         * Should the application close.
         * 
         * \return bool
         */
        CATALYST_LOGIC_DISCARD const bool Close() const;
        /**
         * Set the application to close.
         */
        void Close(const bool);

        /**
         * Pure virtual, your main function.
         * 
         */
        virtual void Run() = 0;

    protected:
        explicit IApplication(const char* process_name);

        /** Static variables and flags */
    private:
        static std::atomic<std::shared_ptr<IApplication>> s_Instance; /** <! s_Instance - Singleton instance of the application */
        static std::atomic_bool s_Reload; /** <! s_Reload - Flag the application to restart and close */

        /** Friends */
    private:
        template<typename T, typename R>
        friend static CatalystResult LaunchApplication();
        friend static void CloseApplication();
        friend static void ReloadApplication();

    private:
        const char* m_Name;
        std::atomic_bool m_Close;
        
    };

    /**
     * Closes and restarts the application.
     * 
     * @brief IMPORTANT: For internal engine use only
     */
    static void ReloadApplication()
    {
        IApplication::Get()->m_Close.store(true);
        IApplication::s_Reload.store(true);
    }

    /**
     * Closes the application and cleans up.
     * 
     * @brief IMPORTANT: For internal engine use only
     */
    static void CloseApplication()
    {
        if (!IApplication::Get())
            return;

        IApplication::Get()->m_Close.store(true);

        IApplication::Get()->~IApplication();
    }

    /**
     * Launches a new application under a new thread.
     * 
     * @brief IMPORTANT: For internal engine use only
     */
    template<typename T, typename R = CatalystEnableIf<CatalystBaseOf<IApplication, T>::value, T>::type>
    CATALYST_LOGIC_DISCARD static CatalystResult LaunchApplication()
    {
        CloseApplication();

        IApplication::s_Instance.store(std::make_shared<R>());

        if(!IApplication::Get())
            return CatalystResult::IApplication_Failed_Creation;

        IApplication::Get()->Run();

        return CatalystResult::Success;
    }
}
