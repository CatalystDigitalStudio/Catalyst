#include "phc.h"
#include "Profiling.h"

#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Catalyst
{
    std::shared_ptr<spdlog::logger> Profile::s_Logger;

    void Profile::initalize(const char* path, const char* pattern)
    {
        s_Logger = spdlog::basic_logger_mt("CATALYST_PROFILE_LOGGER", path);

        if (pattern)
            s_Logger->set_pattern(pattern);
    }
    Profile::Profile(const char* name)
        : m_Name(name)
    {
    }
    Profile::~Profile()
    {
    }
    void Profile::start()
    {
        m_Start = clock::now();
    }
    void Profile::stop()
    {
        m_Stop = clock::now();
    }
    auto Profile::count()
    {
        return (m_Stop - m_Start).count();
    }
    void Profile::log()
    {
        s_Logger->info("[FUNCTION - {0}] | Profile result : {1}", m_Name, this->count());
    }
    ProfileFunction::ProfileFunction(const char* function_name, size_t* result)
        : m_Profile(function_name), m_Result(result)
    {
        m_Profile.start();
    }
    ProfileFunction::~ProfileFunction()
    {
        m_Profile.stop();

        if(m_Result)
            *m_Result = (size_t)m_Profile.count();
         
        m_Profile.log();
    }
}