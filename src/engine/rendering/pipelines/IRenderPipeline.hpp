#ifndef SFA_SRC_ENGINE_RENDERING_PIPELINES_I_RENDER_PIPELINE_HPP
#define SFA_SRC_ENGINE_RENDERING_PIPELINES_I_RENDER_PIPELINE_HPP

#include <glm/glm.hpp>

namespace sfa
{

// TODO: This needs concept to ensure DrawParamT is in RenderCommand optional, see physics/Intersections.hpp
template<typename DrawParamT>
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
    virtual void beginFrame(const glm::mat4& projection) const = 0;
    /// \brief End the current frame.
    virtual void endFrame() const = 0;

    /// \brief Render a \ref TextSubmission to the screen.
    ///
    /// \param submission Data of the submitted text renderable.
    virtual void draw(const DrawParamT& submission) const = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_PIPELINES_I_RENDER_PIPELINE_HPP

