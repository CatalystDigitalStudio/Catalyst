#pragma once

#include "glad.h"

#include "graphic/renderer.h"

namespace Catalyst
{

    class OpenGLProgram
    {


    public:
        explicit OpenGLProgram();
        ~OpenGLProgram();

        void addStage(CatalystShaderStageType stagetype, std::string code);

        bool build();

        const unsigned int& getUniform(std::string name);
        const unsigned int& getID();

    private:
        unsigned int createShader(GLenum type, std::string code);

    private:
        unsigned int m_ID;

        std::vector<unsigned int> m_Shaders;
        std::unordered_map<std::string, unsigned int> m_Uniforms;

    };

    class OpenGLPipline : public IPipeline
    {

    public:
        OpenGLPipline();
        virtual ~OpenGLPipline();

        /**
            * Initalize pipeline.
            */
        virtual void initalize(PipelineInformation) override;

        void makeCurrent();

        /**
         * Destroy pipeline.
         */
        virtual void cleanup() override;

        /**
         * Adds shader stage.
         */
        virtual void addStage(CatalystShaderStageType stagetype, std::string code) override;

        /**
         * Compiles GLSL shader code to byte code.
         */
        virtual std::string compileShader(std::string rawCode) override;

    private:
        PipelineInformation m_pInformation = {};

        OpenGLProgram m_Program = {};

    };

}