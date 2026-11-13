#include "RenderExecutor.hpp"

#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/ISpritePipeline.hpp"
#include "rendering/pipelines/ITextPipeline.hpp"

#include <memory>
#include <span>
#include <utility>
#include <variant>

namespace sfa
{

RenderExecutor::RenderExecutor(
    std::unique_ptr<ISpritePipeline> spritePipeline, std::unique_ptr<ITextPipeline> textPipeline
)
    : m_spritePipeline{ std::move(spritePipeline) }, m_textPipeline{ std::move(textPipeline) }
{}

void RenderExecutor::execute(std::span<const RenderCommand> commands) const
{
    for(const auto& command : commands)
        std::visit([this](const auto& cmd) { executeImpl(cmd); }, command);
}

/// \brief Forward \ref SpriteSubmission to an implementation of \ref ISpritePipeline.
///
/// \param submission The information of the submitted sprite.
void RenderExecutor::executeImpl(const SpriteSubmission& submission) const
{
    m_spritePipeline->draw(submission);
}

/// \brief Forward \ref SpriteSubmission to an implementation of \ref ITextPipeline.
///
/// \param submission The information of the submitted sprite.
void RenderExecutor::executeImpl(const TextSubmission& submission) const
{
    m_textPipeline->draw(submission);
}

} // namespace sfa

