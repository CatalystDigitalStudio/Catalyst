#pragma once

#include "core/internal.h"
#include "core/engine.h"
#include "image.h"

namespace Catalyst
{

    enum CatalystSurfaceFlags
    {
        CATALYST_SURFACE_DEFAULT_POSITION  = 0,
        CATALYST_SURFACE_DEFAULT_DIMENSION = 0
    };

    struct SurfaceData
    {
        std::string m_Title = "Catalyst";
        struct
        {
            unsigned int m_X;
            unsigned int m_Y;
        } m_Position = { CATALYST_SURFACE_DEFAULT_POSITION };
        struct
        {
            unsigned int m_Width;
            unsigned int m_Height;
        } m_Dimension = { CATALYST_SURFACE_DEFAULT_DIMENSION };
    };

    class CATALYST_API ISurface
    {

    public:
        ISurface() = default;
        virtual ~ISurface() = default;

        virtual void create(SurfaceData) = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;

        virtual ISurface* setTitle(const std::string&) = 0;
        virtual ISurface* setIcon(std::shared_ptr<Image>) = 0;
        virtual ISurface* setPosition(unsigned int x, unsigned int y) = 0;
        virtual ISurface* setDimension(unsigned int width, unsigned int height) = 0;

    protected:
        SurfaceData m_Data = {};
    };

    using CatalystPtrSurface = ISurface*;
}
