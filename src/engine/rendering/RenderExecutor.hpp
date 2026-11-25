#ifndef SFA_SRC_ENGINE_RENDERING_RENDER_EXECUTOR_HPP
#define SFA_SRC_ENGINE_RENDERING_RENDER_EXECUTOR_HPP

#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/ISpritePipeline.hpp"
#include "rendering/pipelines/ITextPipeline.hpp"

#include <span>

namespace sfa
{

/// \brief The \ref RenderExecutor is responsible for executing \ref RenderCommand using the appropriate pipelines.
///
/// \author Felix Hommel
/// \date 11/11/2026
class RenderExecutor
{
public:
    RenderExecutor(ISpritePipeline& spritePipeline, ITextPipeline& textPipeline);
    ~RenderExecutor() = default;

    RenderExecutor(const RenderExecutor&) = delete;
    RenderExecutor& operator=(const RenderExecutor&) = delete;
    RenderExecutor(RenderExecutor&&) noexcept = delete;
    RenderExecutor& operator=(RenderExecutor&&) noexcept = delete;

    /// \brief Execute a collection of \ref RenderCommand using rendering pipelines.
    ///
    /// \param commands The collection of \ref RenderCommand
    void execute(std::span<const RenderCommand> commands) const;

private:
    ISpritePipeline& m_spritePipeline;
    ITextPipeline& m_textPipeline;

    void executeImpl(const SpriteSubmission& submission) const;
    void executeImpl(const TextSubmission& submission) const;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_RENDER_EXECUTOR_HPP

