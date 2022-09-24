#include "phc.h"
#include "application.h"

namespace Catalyst
{
    std::atomic<std::shared_ptr<IApplication>> IApplication::s_Instance;
    std::atomic_bool IApplication::s_Reload;

    IApplication::IApplication(const char* process_name)
        : m_Name(process_name), m_Close(false)
    {
        s_Reload.store(false);

        CATALYST_CORE_INFO("*************************\tCATALYST APPLICATION STARTING\t*************************");
    }
    CATALYST_LOGIC_DISCARD std::shared_ptr<IApplication> IApplication::Get()
    {
        return s_Instance;
    }
    IApplication::~IApplication()
    {
        //CATALYST_INFO("*************************\tCATALYST APPLICATION STOPING\t*************************\n");
    }
    const bool IApplication::Close() const
    {
        return m_Close.load();
    }
    void IApplication::Close(const bool close)
    {
        m_Close.store(close);
    }
}