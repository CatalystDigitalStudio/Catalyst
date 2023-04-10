#include "pch.h"
#include "Windows.h"

#include "graphic/renderer.h"

namespace Catalyst
{

    PlatformData WindowsPlatform::getPlatformData()
    {
        return data;
    }

    std::shared_ptr<Platform> initalizePlatform()
    {
        return std::make_shared<WindowsPlatform>();
    }
}