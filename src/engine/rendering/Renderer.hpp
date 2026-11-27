#ifndef SFA_SRC_ENGINE_RENDERING_RENDERER_HPP
#define SFA_SRC_ENGINE_RENDERING_RENDERER_HPP

#include "rendering/IRenderSubmitter.hpp"
#include "rendering/RenderCommand.hpp"
#include "rendering/RenderExecutor.hpp"
#include "rendering/RenderQueue.hpp"
#include "rendering/pipelines/ISpritePipeline.hpp"
#include "rendering/pipelines/ITextPipeline.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace sfa
{

/// \brief The Renderer is the central orchestrator for the rendering operation.
///
/// \author Felix Hommel
/// \date 11/23/2026
class Renderer final : public IRenderSubmitter
{
public:
    /// \brief Create a new \ref Renderer by providing it the pipelines it is going to use.
    ///
    /// \param spritePipeline An implementation of \ref ISpritePipeline
    /// \param textPipeline An implementation of \ref ITextPipeline
    Renderer(std::unique_ptr<ISpritePipeline> spritePipeline, std::unique_ptr<ITextPipeline> textPipeline);
    ~Renderer() override = default;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) noexcept = delete;
    Renderer& operator=(Renderer&&) noexcept = delete;

    /// \brief Begin a new frame.
    ///
    /// \param projection The projection matrix
    ///
    /// \throws std::runtime_error if a frame was already started and has not ended yet.
    void beginFrame(const glm::mat4& projection);
    /// \brief End the current frame.
    ///
    /// \throws std::runtime_error if a frame hasn't been started yet.
    void endFrame();

    /// \brief Submit a new \ref RenderCommand to the \ref Renderer.
    ///
    /// If no frame was started, the command will be discarded.
    ///
    /// \param command The \ref RenderCommand that is submitted
    void submit(RenderCommand command) override;

private:
    std::unique_ptr<ISpritePipeline> m_spritePipeline;
    std::unique_ptr<ITextPipeline> m_textPipeline;

    RenderQueue m_queue;
    RenderExecutor m_executor;

    bool m_inFrame{ false };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_RENDERER_HPP

