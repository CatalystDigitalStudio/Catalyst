#pragma once

#include "core/internal.h"
#include "core/engine.h"
#include "surface.h"

namespace Catalyst
{

    typedef unsigned int DeviceID;

    enum CatalystRendererType
    {
        CATALYST_RENDERER_TYPE_NONE,
        CATALYST_RENDERER_TYPE_VULKAN,
        CATALYST_RENDERER_TYPE_DX11,
        CATALYST_RENDERER_TYPE_DX12
    };

    enum CatalystRendererFlags
    {
        CATALYST_RENDERER_FLAG_NONE          = 0x0000,
        CATALYST_RENDERER_FLAG_HEADLESS      = 0x0002,

        CATALYST_RENDERER_FLAG_DEVICE_DEFAULT     = 0x0010,
        CATALYST_RENDERER_FLAG_DEVICE_LOW_POWER   = 0x0011,
        CATALYST_RENDERER_FLAG_DEVICE_INTEGERATED = 0x0012,
                          
        CATALYST_RENDERER_FLAG_SINGLE_BUFFER = 0x0021,
        CATALYST_RENDERER_FLAG_DOUBLE_BUFFER = 0x0022
    };

    enum CatalystShaderStageType
    {
        CATALYST_SHADER_STAGE_UNKNOWN = 0,
        CATALYST_SHADER_STAGE_GEOMETERY,
        CATALYST_SHADER_STAGE_FRAGMENT,
        CATALYST_SHADER_STAGE_TESSELATION
    };

    enum CatalystShaderTopology
    {
        CATALYST_SHADER_TOPOLOGY_POINT_LIST,
        CATALYST_SHADER_TOPOLOGY_LINE_LIST,
        CATALYST_SHADER_TOPOLOGY_LINE_STRIP,
        CATALYST_SHADER_TOPOLOGY_TRIANGLE_FAN,
        CATALYST_SHADER_TOPOLOGY_TRIANGLE_LIST,
        CATALYST_SHADER_TOPOLOGY_TRIANGLE_STRIP,
    };

    enum CatalystCullFace
    {
        CATALYST_SHADER_CULL_FACE_FRONT,
        CATALYST_SHADER_CULL_FACE_BACK
    };

    enum CatalystCullDirection
    {
        CATALYST_SHADER_CULL_DIRECTION_CLOCKWISE,
        CATALYST_SHADER_CULL_DIRECTION_COUNTER_CLOCKWISE
    };

    struct PipelineInformation
    {
        /**
         * Which side of a face should the shader cull. 
         * Default - Back 
         */
        CatalystCullFace cullFace = CATALYST_SHADER_CULL_FACE_BACK;
        /**
         * Which direction is a face constructed. 
         * Default - Clockwise
         */
        CatalystCullDirection cullDirection = CATALYST_SHADER_CULL_DIRECTION_CLOCKWISE;

        /**
         * How are verticies treated? Lines, Faces, etc. 
         * Default - Triangle Strip
         */
        CatalystShaderTopology topology = CATALYST_SHADER_TOPOLOGY_TRIANGLE_STRIP;

        uint32_t imageWidth = 0;
        uint32_t imageHeight = 0;
    };

    /**
     * Pipeline(Shader)
     * 
     * @brief An object used to contain and work on a pipeline.
     */
    class IPipeline
    {

    public:
        

    public:
        IPipeline() = default;
        virtual ~IPipeline() = default;

        /**
         * Initalize pipeline.
         */
        virtual void initalize(PipelineInformation) = 0;
        /**
         * Destroy pipeline.
         */
        virtual void cleanup() = 0;

        /**
         * Adds shader stage.
         */
        virtual void addStage(CatalystShaderStageType stagetype, std::string byteCode) = 0;

        /**
         * Compiles GLSL shader code to byte code.
         */
        virtual std::string compileShader(std::string rawCode) = 0;

    private:
        std::vector<std::string> m_ByteCodes;

    };

    class CATALYST_API ISwapChain
    {

    public:
        virtual ~ISwapChain() = default;

        virtual void initalize() = 0;

    };

    struct CATALYST_API RendererInfo
    {
        int type = CATALYST_RENDERER_TYPE_NONE;
        int flags = (CATALYST_RENDERER_FLAG_DEVICE_DEFAULT | CATALYST_RENDERER_FLAG_DOUBLE_BUFFER);
    };

    class CATALYST_API IRenderer
    {

    public:
        virtual ~IRenderer();

        virtual void initalize() = 0;
        virtual void cleanup() = 0;

        virtual void createPipeline() = 0;
        virtual std::shared_ptr<IPipeline> getPipeline() = 0;
    
    protected:
        IRenderer();

    protected:
        RendererInfo m_Info;
    };

    using CatalystPtrRenderer = IRenderer*;
}