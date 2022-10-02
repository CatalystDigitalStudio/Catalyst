#pragma once

#include "core/internal.h"
#include "surface.h"

namespace Catalyst
{

    typedef unsigned int DeviceID;

    class CATALYST_API IDevice
    {

    public:
        enum class Property
        {
            HighPerformanceMode
        };

    public:
        IDevice() = default;
        virtual ~IDevice() = default;

        virtual const DeviceID getID() = 0;
        virtual const bool getProperty(Property) = 0;

        virtual void setProperty(Property, bool value) = 0;

    };

    using device_ptr = std::shared_ptr<IDevice>;

    class CATALYST_API IRenderer
    {

    public:
        virtual ~IRenderer();

        virtual void initalize(std::shared_ptr<ISurface>) = 0;
        virtual void cleanup() = 0;

        virtual void addPipline() = 0;

    public:
        const std::unordered_map<DeviceID, device_ptr> enumerateDevices();
        const device_ptr getDevices(DeviceID);

    protected:
        IRenderer();

        virtual void initalizeDevices() = 0;
        virtual void initalizeSwapchain() = 0;

    protected:
        std::shared_ptr<ISurface> m_Surface;
        std::unordered_map<DeviceID, device_ptr> m_Devices;

    };

    using renderer_ptr = std::shared_ptr<IRenderer>;
}