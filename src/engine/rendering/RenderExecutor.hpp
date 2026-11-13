#ifndef SFA_SRC_ENGINE_RENDERING_RENDER_EXECUTOR_HPP
#define SFA_SRC_ENGINE_RENDERING_RENDER_EXECUTOR_HPP

#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/ISpritePipeline.hpp"

#include <memory>
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
    RenderExecutor(std::unique_ptr<ISpritePipeline> spritePipeline);
    ~RenderExecutor() = default;

    RenderExecutor(const RenderExecutor&) = delete;
    RenderExecutor& operator=(const RenderExecutor&) = delete;
    RenderExecutor(RenderExecutor&&) noexcept = delete;
    RenderExecutor& operator=(RenderExecutor&&) noexcept = delete;

    void execute(std::span<const RenderCommand> commands) const;

private:
    std::unique_ptr<ISpritePipeline> m_spritePipeline;

    void executeImpl(const SpriteSubmission& submission) const;
    void executeImpl(const TextSubmission& submission) const;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_RENDER_EXECUTOR_HPP

