#ifndef SFA_SRC_ENGINE_RENDERING_PIPELINES_I_TEXT_PIPELINE_HPP
#define SFA_SRC_ENGINE_RENDERING_PIPELINES_I_TEXT_PIPELINE_HPP

#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/IRenderPipeline.hpp"

#include <glm/glm.hpp>

namespace sfa
{

/// \brief Interface definition for text renderers.
///
/// \author Felix Hommel
/// \date 11/13/2026
class ITextPipeline : public IRenderPipeline<TextSubmission>
{
public:
    ITextPipeline() = default;
    ~ITextPipeline() override = default;

    ITextPipeline(const ITextPipeline&) = delete;
    ITextPipeline& operator=(const ITextPipeline&) = delete;
    ITextPipeline(ITextPipeline&&) noexcept = delete;
    ITextPipeline& operator=(ITextPipeline&&) noexcept = delete;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_PIPELINES_I_TEXT_PIPELINE_HPP

