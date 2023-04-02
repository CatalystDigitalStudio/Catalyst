#pragma once

#include "core/internal.h"
#include "core/engine.h"
#include "surface.h"
#include "commands.h"

namespace Catalyst
{

    typedef unsigned int DeviceID;

    enum CatalystRendererType
    {
        CATALYST_RENDERER_TYPE_NONE,
        CATALYST_RENDERER_TYPE_VULKAN,
        CATALYST_RENDERER_TYPE_OPENGL,
        CATALYST_RENDERER_TYPE_DX11,
        CATALYST_RENDERER_TYPE_DX12
    };

    enum CatalystRendererFlags
    {
        /* NULL FLAG */
        CATALYST_RENDERER_FLAG_NONE          = 0x0000,
        /* USES NO PRESENTATION SURFACE */
        CATALYST_RENDERER_FLAG_HEADLESS      = 0x0002,

        /* LOOKS FOR MOST SUITABLE DEVICE TO RENDER */
        CATALYST_RENDERER_FLAG_DEVICE_DEFAULT     = 0x0010,
        /* IS LOW POWER CONSUMSION DEVICE */
        CATALYST_RENDERER_FLAG_DEVICE_LOW_POWER   = 0x0011,
        /* IS INTEGERATED GRAPHICS DEVICE */
        CATALYST_RENDERER_FLAG_DEVICE_INTEGERATED = 0x0012,
                          
        /* USE A SINGLE PRESENTATION BUFFER*/
        CATALYST_RENDERER_FLAG_SINGLE_BUFFER = 0x0021,
        /* USE A FRONT AND BACK PRESENTATION BUFFER*/
        CATALYST_RENDERER_FLAG_DOUBLE_BUFFER = 0x0022,
        /* USE A FRONT AND 2 BACK PRESENTATION BUFFER*/
        CATALYST_RENDERER_FLAG_TRIPLE_BUFFER = 0x0023
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
     * .
     */
    typedef unsigned int PipelineID;

    /**
     * Pipeline(Shader)
     * 
     * @brief An object used to contain and work on a pipeline.
     */
    class CATALYST_API IPipeline
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

    };

    using PipelinePtr = std::shared_ptr<IPipeline>;

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

    class CATALYST_API IDevice
    {

    public:
        IDevice(const std::string& name);
        virtual ~IDevice() = default;

        const std::string& getName() const;

    private:
        const std::string m_Name;

    };

    class CATALYST_API IRenderer
    {

    public:
        virtual ~IRenderer();

        virtual void initalize() = 0;
        virtual void cleanup() = 0;

        virtual void render() = 0;

    public:
        virtual const std::vector<IDevice*> getDeviceList() = 0;

    public:
        virtual PipelineID createPipeline(PipelineInformation) = 0;
        virtual std::shared_ptr<IPipeline> getPipeline(PipelineID) = 0;

        virtual constexpr CommandPool& getCommandPool() = 0;
    
    protected:
        IRenderer();

    protected:
        RendererInfo m_Info;
    };

    using CatalystPtrRenderer = IRenderer*;
}





