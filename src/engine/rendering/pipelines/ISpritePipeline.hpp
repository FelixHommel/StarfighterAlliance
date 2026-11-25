#ifndef SFA_SRC_ENGINE_RENDERING_PIPELINES_I_SPIRTE_PIPELINE_HPP
#define SFA_SRC_ENGINE_RENDERING_PIPELINES_I_SPIRTE_PIPELINE_HPP

#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/IRenderPipeline.hpp"

#include <glm/glm.hpp>

namespace sfa
{

/// \brief Interface definition for sprite renderers.
///
/// \author Felix Hommel
/// \date 11/13/2026
class ISpritePipeline : public IRenderPipeline<SpriteSubmission>
{
public:
    ISpritePipeline() = default;
    ~ISpritePipeline() override = default;

    ISpritePipeline(const ISpritePipeline&) = delete;
    ISpritePipeline& operator=(const ISpritePipeline&) = delete;
    ISpritePipeline(ISpritePipeline&&) noexcept = delete;
    ISpritePipeline& operator=(ISpritePipeline&&) noexcept = delete;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_PIPELINES_I_SPIRTE_PIPELINE_HPP

