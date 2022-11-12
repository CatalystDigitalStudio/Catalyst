#include "pch.h"
#include "commands.h"

namespace Catalyst
{
    CommandPool::CommandPool()
    {
    }
    CommandPool::~CommandPool()
    {
    }
    void CommandPool::add(std::shared_ptr<RenderCommandBase> command)
    {
        m_Commands.push_front(command);
    }
    std::shared_ptr<RenderCommandBase> CommandPool::next()
    {
        auto command = m_Commands.back();
        m_Commands.pop_back();
        return command;
    }
}
