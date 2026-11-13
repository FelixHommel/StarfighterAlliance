#ifndef SFA_SRC_ENGINE_RENDERING_PIPELINES_I_TEXT_PIPELINE_HPP
#define SFA_SRC_ENGINE_RENDERING_PIPELINES_I_TEXT_PIPELINE_HPP

#include "rendering/RenderCommand.hpp"

namespace sfa
{

/// \brief Interface definition for text renderers.
///
/// \author Felix Hommel
/// \date 11/13/2026
class ITextPipeline
{
public:
    ITextPipeline() = default;
    virtual ~ITextPipeline() = default;

    ITextPipeline(const ITextPipeline&) = delete;
    ITextPipeline& operator=(const ITextPipeline&) = delete;
    ITextPipeline(ITextPipeline&&) noexcept = delete;
    ITextPipeline& operator=(ITextPipeline&&) noexcept = delete;

    /// \brief Render a \ref TextSubmission to the screen.
    ///
    /// \param submission Data of the submitted text renderable.
    virtual void draw(const TextSubmission& submission) const = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_PIPELINES_I_TEXT_PIPELINE_HPP

