#pragma once

#include "engine.h"

namespace Catalyst
{

    class IApplication;

    class Scene
    {

    public:
        explicit Scene(std::string name, IApplication* app);
        virtual ~Scene() = default;

        virtual void onCreate() = 0;
        virtual void onUpdate() = 0;
        virtual void onDestroy() = 0;

    public:
        const std::string& getName();

    protected:
        const IApplication* getApplication();
        template<typename T>
        inline const T* getApplication() { return (T*)getApplication(); }

    private:
        std::string m_Name = "null";

    private:
        IApplication* m_App = nullptr;

    };

}
