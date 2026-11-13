#include "RenderExecutor.hpp"

#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/ISpritePipeline.hpp"

#include <memory>
#include <utility>
#include <variant>

namespace sfa
{

RenderExecutor::RenderExecutor(std::unique_ptr<ISpritePipeline> spritePipeline)
    : m_spritePipeline{ std::move(spritePipeline) }
{}

void RenderExecutor::execute(std::span<const RenderCommand> commands) const
{
    for(const auto& command : commands)
        std::visit([this](const auto& cmd) { executeImpl(cmd); }, command);
}

void RenderExecutor::executeImpl(const SpriteSubmission& submission) const
{
    m_spritePipeline->draw(submission);
}

void RenderExecutor::executeImpl(const TextSubmission& submission) const {}

} // namespace sfa

