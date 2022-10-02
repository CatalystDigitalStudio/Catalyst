#pragma once

#include <string>
#include <memory>

#include "../src/core/internal.h"


namespace Catalyst
{

    struct PlatformData;

    class Platform
    {

    public:
        virtual PlatformData getPlatformData() = 0;

        virtual std::shared_ptr<IRenderer> initalizeRenderer(std::string) = 0;

    };

    extern std::shared_ptr<Platform> initalizePlatform();

}
