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
    void CommandPool::reserve(unsigned int amount)
    {
        if(amount < m_Commands.size())
            m_Commands.resize(amount);
    }
    void CommandPool::flush()
    {
        m_Commands.erase(m_Commands.begin(), m_Commands.end());
    }
    std::shared_ptr<RenderCommandBase> CommandPool::next()
    {
        auto command = m_Commands.back();
        m_Commands.pop_back();
        return command;
    }
}
