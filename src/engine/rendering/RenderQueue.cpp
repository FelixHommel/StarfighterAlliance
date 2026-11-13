#include "RenderQueue.hpp"

#include "rendering/RenderCommand.hpp"

#include <utility>

namespace sfa
{

void RenderQueue::submit(RenderCommand cmd)
{
    m_commands.push_back(std::move(cmd));
}

void RenderQueue::clear()
{
    m_commands.clear();
}


} // namespace sfa

