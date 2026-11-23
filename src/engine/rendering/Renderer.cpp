#include "Renderer.hpp"

#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/ISpritePipeline.hpp"
#include "rendering/pipelines/ITextPipeline.hpp"
#include "utility/PreCondition.hpp"

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <memory>
#include <stdexcept>
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
    pre::require<std::runtime_error>(!m_inFrame, "Cannot start a new frame. A frame is already active");

    m_inFrame = true;

    m_spritePipeline->beginFrame(projection);
    m_textPipeline->beginFrame(projection);
}

void Renderer::endFrame()
{
    pre::require<std::runtime_error>(m_inFrame, "Cannot end a frame if no frame is currently being rendered");

    m_spritePipeline->endFrame();
    m_textPipeline->endFrame();

    if(m_queue.empty())
        return;

    m_executor.execute(m_queue.takeCommands());

    m_inFrame = false;
}

void Renderer::submit(RenderCommand command)
{
    if(!m_inFrame)
    {
        spdlog::warn(
            "Submitting new commands is only allowed when Renderer::beginFrame(...) was called beforehand. This command will be discarded"
        );
        return;
    }

    m_queue.submit(std::move(command));
}

} // namespace sfa

