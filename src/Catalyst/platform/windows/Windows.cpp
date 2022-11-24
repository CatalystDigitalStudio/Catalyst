#include "pch.h"
#include "Windows.h"

#include "graphic/renderer.h"

#include "windowsSurface.h"

namespace Catalyst
{

    PlatformData WindowsPlatform::getPlatformData()
    {
        return data;
    }

    CatalystPtrSurface WindowsPlatform::createSurface(SurfaceData data)
    {
        CatalystPtrSurface surface = nullptr;

        surface = new WindowsSurface();
        
        surface->create(data);

        return surface;
    }

    std::shared_ptr<Platform> initalizePlatform()
    {
        return std::make_shared<WindowsPlatform>();
    }
}