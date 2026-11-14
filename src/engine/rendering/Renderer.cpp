#include "Renderer.hpp"

#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/ISpritePipeline.hpp"

#include <memory>
#include <utility>

namespace sfa
{

Renderer::Renderer(std::unique_ptr<ISpritePipeline> spriteRenderer) : m_executor{ std::move(spriteRenderer) } {}

void Renderer::beginFrame() {}

void Renderer::endFrame()
{
    m_executor.execute(m_queue.commands());
}

void Renderer::submit(RenderCommand command)
{
    m_queue.submit(std::move(command));
}

} // namespace sfa

