#include "pch.h"

namespace Catalyst
{
    Surface::Surface()
    {
    }
    Surface::~Surface()
    {
    }
    void Surface::create(std::string title, unsigned int width, unsigned int height)
    {
        m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    }
    void Surface::update()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }
    void Surface::destroy()
    {
        glfwDestroyWindow(m_Window);
    }
    Surface Surface::setTitle(const std::string& title)
    {
        glfwSetWindowTitle(m_Window, title.c_str());
        return (*this);
    }
    Surface Surface::setIcon(std::shared_ptr<Image>)
    {
        return (*this);
    }
    Surface Surface::setPosition(unsigned int x, unsigned int y)
    {
        return (*this);
    }
    Surface Surface::setDimension(unsigned int width, unsigned int height)
    {
        return (*this);
    }
}
