#include "pch.h"
#include "application.h"

namespace Catalyst
{
    std::atomic<std::shared_ptr<IApplication>> IApplication::s_Instance;
    std::atomic_bool IApplication::s_Reload;
    std::unique_ptr<Scene> m_Scene;

    IApplication::IApplication(const char* process_name)
        : m_Name(process_name), m_Close(false)
    {
        s_Reload.store(false);

        CATALYST_CORE_INFO("*************************\tCATALYST APPLICATION STARTING\t*************************");
    }
    CATALYST_LOGIC_DISCARD std::shared_ptr<IApplication> IApplication::get()
    {
        return s_Instance;
    }
    IApplication::~IApplication()
    {
        //CATALYST_INFO("*************************\tCATALYST APPLICATION STOPING\t*************************\n");
    }
    const bool IApplication::close() const
    {
        return m_Close.load();
    }
    const char* IApplication::name() const
    {
        return m_Name;
    }
    CATALYST_LOGIC_DISCARD Scene* IApplication::getScene()
    {
        return m_Scene.get();
    }
    void IApplication::closeScene()
    {
        if (m_Scene)
        {
            m_Scene->onDestroy();
            m_Scene.reset();
        }
    }
    void IApplication::close(const bool close)
    {
        m_Close.store(close);
    }
}