#pragma once

#include "internal.h"
#include "spdlog/logger.h"

#include <chrono>

#ifndef CATALYST_HEADER_PROFILE
#define CATALYST_HEADER_PROFILE

namespace Catalyst
{

    class Profile
    {

        static std::shared_ptr<spdlog::logger> s_Logger;

    private:
        static void initalize(const char* path, const char* pattern = nullptr);

    private:
        friend void CatalystInitalizeEngine(int argc, char** argv, char** envp);

    public:
        using clock = std::chrono::high_resolution_clock;
        using point = std::chrono::high_resolution_clock::time_point;

    public:

        explicit Profile(const char* name);
        ~Profile();

        void start();
        void stop();

        auto count();

        void log();

    private:
        point m_Start;
        point m_Stop;

        const char* m_Name;
    };

    class ProfileFunction
    {

    public:
        explicit ProfileFunction(const char* function_name, size_t* result);
        ~ProfileFunction();
    private:
        Profile m_Profile;
        size_t* m_Result;

    };

}

#endif //CATALYST_HEADER_PROFILE