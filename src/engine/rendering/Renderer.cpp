#include "Renderer.hpp"

#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/ISpritePipeline.hpp"
#include "rendering/pipelines/ITextPipeline.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <utility>

namespace sfa
{

Renderer::Renderer(std::unique_ptr<ISpritePipeline> spritePipeline, std::unique_ptr<ITextPipeline> textPipeline)
    : m_spritePipeline{ std::move(spritePipeline) }
    , m_textPipeline{ std::move(textPipeline) }
    , m_executor{ *m_spritePipeline, *m_textPipeline }
{}

void Renderer::beginFrame(const glm::mat4& projection)
{
    m_spritePipeline->beginFrame(projection);
    m_textPipeline->beginFrame(projection);
}

void Renderer::endFrame()
{
    if(m_queue.empty())
        return;

    m_executor.execute(m_queue.commands());
    m_queue.clear();
}

void Renderer::submit(RenderCommand command)
{
    m_queue.submit(std::move(command));
}

} // namespace sfa

