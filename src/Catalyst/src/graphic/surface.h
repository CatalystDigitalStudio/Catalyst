#pragma once

#include "core/internal.h"
#include "core/engine.h"
#include "image.h"

#include "GLFW/glfw3.h"

namespace Catalyst
{


    class CATALYST_API Surface
    {

    public:
        Surface();
        ~Surface();

        void create(std::string title = "Catalyst", unsigned int width = 0, unsigned int height = 0);
        void update();
        void destroy();

        Surface setTitle(const std::string&);
        Surface setIcon(std::shared_ptr<Image>);
        Surface setPosition(unsigned int x, unsigned int y);
        Surface setDimension(unsigned int width, unsigned int height);

    protected:
        GLFWwindow* m_Window;

    };

}
