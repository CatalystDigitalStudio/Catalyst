#include "OpenGLRenderer.h"

#include "core/internal.h"

#include "glad.h"

static void catalystDebugMessageCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    {
        CATALYST_CORE_WARN("OpenGL Warn : {0}", message);
        break;
    }
    case GL_DEBUG_TYPE_ERROR:
    {
        CATALYST_CORE_ERROR("OpenGL Error : {0}", message);
        break;
    }
    }
}

extern "C" Catalyst::CatalystResult createOpenGLRenderer(Catalyst::IRenderer * *renderer, Catalyst::CatalystPtrSurface surface, Catalyst::RendererInfo info)
{
    *renderer = new OpenGLRenderer();
     
    if (renderer)
    {
        return Catalyst::CatalystResult::Success;
    }
    else
    {
        return Catalyst::CatalystResult::Pointer_Is_Nullptr;
    }
}

void OpenGLRenderer::initalize()
{
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*              ONE-TIME GLAD INITALIZATION               */
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    static int gladInitalized = 0;

    if (!gladInitalized)
    {
        gladInitalized = gladLoadGL();

        if (!gladInitalized)
        {
            throw;
        }

    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /*                ONE-TIME INITALIZATION                  */
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#ifdef CATALYST_DEBUG
    glEnable(GL_DEBUG_OUTPUT);

    glDebugMessageCallback(catalystDebugMessageCallback, nullptr);
#endif

}

void OpenGLRenderer::cleanup()
{
}

void OpenGLRenderer::render()
{
}

const std::vector<Catalyst::IDevice*> OpenGLRenderer::getDeviceList()
{
    return std::vector<Catalyst::IDevice*>();
}

Catalyst::PipelineID OpenGLRenderer::createPipeline(Catalyst::PipelineInformation)
{
    return Catalyst::PipelineID();
}

std::shared_ptr<Catalyst::IPipeline> OpenGLRenderer::getPipeline(Catalyst::PipelineID)
{
    return std::shared_ptr<Catalyst::IPipeline>();
}

Catalyst::CommandPool& OpenGLRenderer::getCommandPool()
{
    return m_CommandPool;
}

OpenGLRenderer::~OpenGLRenderer()
{
}

OpenGLRenderer::OpenGLRenderer()
{
}