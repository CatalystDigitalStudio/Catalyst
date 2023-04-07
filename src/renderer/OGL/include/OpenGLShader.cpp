#include "pch.h"
#include "OpenGLShader.h"

namespace Catalyst
{
    OpenGLPipline::OpenGLPipline()
    {
    }
    OpenGLPipline::~OpenGLPipline()
    {
    }
    void OpenGLPipline::initalize(PipelineInformation information)
    {
        m_pInformation = information;
    }
    void OpenGLPipline::makeCurrent()
    {

        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        /*                       CULLING                          */
        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        switch (m_pInformation.cullDirection)
        {
        case CATALYST_SHADER_CULL_DIRECTION_CLOCKWISE:
        {
            glFrontFace(GL_CW);
            break;
        }
        case CATALYST_SHADER_CULL_DIRECTION_COUNTER_CLOCKWISE:
        {
            glFrontFace(GL_CCW);
            break;
        }
        }

        switch (m_pInformation.cullFace)
        {
        case CATALYST_SHADER_CULL_FACE_FRONT:
        {
            glCullFace(GL_FRONT);
            break;
        }
        case CATALYST_SHADER_CULL_FACE_BACK:
        {
            glCullFace(GL_BACK);
            break;
        }
        }

        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        /*                       VIEWPORT                         */
        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        glViewport(0, 0, m_pInformation.imageWidth, m_pInformation.imageHeight);

        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
        /*                       PROGRAM                          */
        /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

        glUseProgram(m_Program.getID());
        

    }
    void OpenGLPipline::cleanup()
    {
        m_Program.~OpenGLProgram();
    }
    void OpenGLPipline::addStage(CatalystShaderStageType stagetype, std::string code)
    {
        m_Program.addStage(stagetype, code);
    }
    std::string OpenGLPipline::compileShader(std::string rawCode)
    {
        return std::string();
    }
    OpenGLProgram::OpenGLProgram()
    {
        m_ID = glCreateProgram();
    }
    OpenGLProgram::~OpenGLProgram()
    {
        glDeleteProgram(m_ID);
    }
    void OpenGLProgram::addStage(CatalystShaderStageType stagetype, std::string code)
    {

        GLenum type = 0;

        switch (stagetype)
        {
        case CATALYST_SHADER_STAGE_GEOMETERY:
        {
            type = GL_VERTEX_SHADER;
            break;
        }
        case CATALYST_SHADER_STAGE_FRAGMENT:
        {
            type = GL_FRAGMENT_SHADER;
            break;
        }
        case CATALYST_SHADER_STAGE_UNKNOWN:
        default:
        {
            return;
        }
        }

    }
    bool OpenGLProgram::build()
    {
        glLinkProgram(m_ID);

        for (unsigned int& id : m_Shaders)
        {
            glDetachShader(m_ID, id);
            glDeleteShader(id);
        }

        int success = 0;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &success);
            char* buffer = new char[success];
            glGetProgramInfoLog(m_ID, success, NULL, buffer);
            CATALYST_CORE_ERROR("OpenGL Shader Linking Error: {0}\n\nSHADER IS UNABLE TO BE USED! Fix error and try reloading!", buffer);
            glDeleteProgram(m_ID);
            return false;
        }

        return true;
    }
    const unsigned int& OpenGLProgram::getUniform(std::string name)
    {
        if (m_Uniforms.find(name) != m_Uniforms.end())
        {
            return m_Uniforms[name];
        }
        else
        {
            return (m_Uniforms[name] = glGetUniformLocation(m_ID, name.c_str()));
        }
    }
    const unsigned int& OpenGLProgram::getID()
    {
        return m_ID;
    }
    unsigned int OpenGLProgram::createShader(GLenum type, std::string code)
    {
        const char* const data = code.c_str();
        unsigned int id = glCreateShader(type);
        glShaderSource(id, 1, &data, NULL);
        glCompileShader(id);

        int success = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &success);
            char* buffer = new char[success];
            glGetShaderInfoLog(id, success, NULL, buffer);
            CATALYST_CORE_ERROR("OpenGL Shader Compilation Error: {0}", buffer);
            glDeleteShader(id);
            return -1;
        }

        glAttachShader(m_ID, id);

        return id;
    }
}