#pragma once

#include <memory>
#include <deque>

namespace Catalyst
{
    class IPipeline;
    class IRenderer;

    typedef unsigned int PipelineID;

    enum RenderCommandType
    {

        CATALYST_RENDER_COMMAND_BIND_PIPELINE

    };

    struct RenderCommandBase
    {
        RenderCommandType type = {};
    };

    template<RenderCommandType TYPE>
    struct RenderCommand : public RenderCommandBase
    {

    };

    struct BindPipelineCommand : public RenderCommand<CATALYST_RENDER_COMMAND_BIND_PIPELINE>
    {
        BindPipelineCommand(Catalyst::PipelineID id)
            : id(id)
        {
        
        }
        ~BindPipelineCommand()
        {
        
        }

        Catalyst::PipelineID id = 0;
    };


    class CommandPool
    {

    public:
        CommandPool();
        ~CommandPool();

        void add(std::shared_ptr<RenderCommandBase> command);
        void reserve(unsigned int amount);
        void flush();
        
    private:
        friend class IRenderer;

        std::shared_ptr< RenderCommandBase> next();

    private:
        std::deque<std::shared_ptr<RenderCommandBase>> m_Commands;
    };

}
