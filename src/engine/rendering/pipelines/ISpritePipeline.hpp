#ifndef SFA_SRC_ENGINE_RENDERING_PIPELINES_I_SPIRTE_PIPELINE_HPP
#define SFA_SRC_ENGINE_RENDERING_PIPELINES_I_SPIRTE_PIPELINE_HPP

#include "rendering/RenderCommand.hpp"

#include <glm/glm.hpp>

namespace sfa
{

/// \brief Interface definition for sprite renderers.
///
/// \author Felix Hommel
/// \date 11/13/2026
class ISpritePipeline
{
public:
    ISpritePipeline() = default;
    virtual ~ISpritePipeline() = default;

    ISpritePipeline(const ISpritePipeline&) = delete;
    ISpritePipeline& operator=(const ISpritePipeline&) = delete;
    ISpritePipeline(ISpritePipeline&&) noexcept = delete;
    ISpritePipeline& operator=(ISpritePipeline&&) noexcept = delete;

    /// \brief Start a new frame.
    ///
    /// \param projection The projection matrix
    virtual void beginFrame(const glm::mat4& projection) const = 0;
    /// \brief End the current frame.
    virtual void endFrame() const = 0;

    /// \brief Render a \ref SpriteSubmission to the screen.
    ///
    /// \param submission Data of the submitted sprite renderable.
    virtual void draw(const SpriteSubmission& submission) const = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_PIPELINES_I_SPIRTE_PIPELINE_HPP

