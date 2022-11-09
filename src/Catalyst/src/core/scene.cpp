#include "pch.h"
#include "scene.h"

namespace Catalyst
{
    Scene::Scene(std::string name, IApplication* app)
        : m_Name(name), m_App(app)
    {
    }

    const std::string& Scene::getName()
    {
        return m_Name;
    }

    const IApplication* Scene::getApplication()
    {
        return m_App;
    }

}