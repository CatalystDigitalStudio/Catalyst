#pragma once

#include "core/internal.h"
#include "image.h"

namespace Catalyst
{

    typedef struct SurfaceData SurfaceData;

    class CATALYST_API ISurface
    {

    public:
        ISurface() = default;
        virtual ~ISurface() = default;

        virtual void create() = 0;
        virtual void update() = 0;
        virtual void destroy() = 0;

        virtual ISurface* setTitle(const std::string&) = 0;
        virtual ISurface* setIcon(std::shared_ptr<Image>) = 0;
        virtual ISurface* setPosition(unsigned int x, unsigned int y) = 0;
        virtual ISurface* setDimension(unsigned int width, unsigned int height) = 0;

    protected:
        SurfaceData* m_Data;
        std::string m_Title;
        unsigned int m_X;
        unsigned int m_Y;
        unsigned int m_Width;
        unsigned int m_Height;

    };

}
