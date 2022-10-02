#pragma once

#include "utilities.h"

namespace Catalyst
{
    

    struct Version
    {
        static constexpr unsigned int major = 0;
        static constexpr unsigned int minor = 0;
        static constexpr unsigned int patch = 0;
        static constexpr const char const version[22] = { 0 };
    };

    template<unsigned int major, unsigned int minor, unsigned int patch>
    static constexpr const Version createVersion()
    {
        Version version = { major, minor, patch, "" };

    }

}
