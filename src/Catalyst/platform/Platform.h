#pragma once

#include <string>
#include <memory>

#include "core/engine.h"
#include "graphic/renderer.h"
#include "graphic/surface.h"


namespace Catalyst
{

    struct PlatformData;

    class CATALYST_API Platform
    {

    public:
        virtual PlatformData getPlatformData() = 0;

        virtual CatalystPtrSurface createSurface(SurfaceData) = 0;

    };

    extern std::shared_ptr<Platform> initalizePlatform();

}
