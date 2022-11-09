#include "pch.h"
#include "Profiling.h"

#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Catalyst
{
    std::shared_ptr<spdlog::logger> Profiler::s_Logger;

    void Profiler::initalize(const char* path, const char* pattern)
    {
        s_Logger = spdlog::basic_logger_mt("CATALYST_PROFILE_LOGGER", path, true);

        if (pattern)
            s_Logger->set_pattern(pattern);

#ifdef CATALYST_PROFILE_LOG_CSV
        s_Logger->info("\"Tick Index\",\"Function\",\"Time NS\"");
#endif
    }
    Profiler::Profiler(const char* name)
        : m_Name(name)
    {
    }
    Profiler::~Profiler()
    {
    }
    void Profiler::start()
    {
        m_Start = clock::now();
    }
    void Profiler::stop()
    {
        m_Stop = clock::now();
    }
    long long Profiler::count()
    {
        return (m_Stop - m_Start).count();
    }
    void Profiler::log()
    {
#ifdef CATALYST_PROFILE_LOG_CSV
        s_Logger->info("\"{0}\",\"{1}\",\"{2}\"", Engine::getTickCount(), m_Name, this->count());
#else
        s_Logger->info("[Function - {0}] - {1}", m_Name, this->count());
#endif
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