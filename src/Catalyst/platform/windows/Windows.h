#pragma once

#include "../Platform.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace Catalyst
{

    struct PlatformData
    {
        inline static const HINSTANCE instance = (HINSTANCE)LoadLibraryW(NULL);
    };

    class WindowsPlatform : public Platform
    {

    public:
        virtual PlatformData getPlatformData() override;

    private:
        PlatformData data = {};

    };
}
