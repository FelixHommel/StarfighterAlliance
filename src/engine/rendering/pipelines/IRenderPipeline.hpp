#ifndef SFA_SRC_ENGINE_RENDERING_PIPELINES_I_RENDER_PIPELINE_HPP
#define SFA_SRC_ENGINE_RENDERING_PIPELINES_I_RENDER_PIPELINE_HPP

#include "rendering/RenderCommand.hpp"
#include "utility/concepts/VariantAlternative.hpp"

#include <glm/glm.hpp>

namespace sfa
{

/// \brief Interface definition for any type of render pipeline.
///
/// \author Felix Hommel
/// \date 11/23/2026
template<concepts::VariantAlternative<RenderCommand> DrawParamT>
class IRenderPipeline
{
public:
    IRenderPipeline() = default;
    virtual ~IRenderPipeline() = default;

    IRenderPipeline(const IRenderPipeline&) = delete;
    IRenderPipeline& operator=(const IRenderPipeline&) = delete;
    IRenderPipeline(IRenderPipeline&&) noexcept = delete;
    IRenderPipeline& operator=(IRenderPipeline&&) noexcept = delete;

    /// \brief Start a new frame.
    ///
    /// \param projection The projection matrix
    virtual void beginFrame(const glm::mat4& projection) = 0;
    /// \brief End the current frame.
    virtual void endFrame() = 0;

    /// \brief Render an instance of \p DrawParamT to the screen.
    ///
    /// \param submission Data of the submitted text renderable.
    virtual void draw(const DrawParamT& submission) = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_PIPELINES_I_RENDER_PIPELINE_HPP

