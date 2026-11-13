#ifndef SFA_SRC_ENGINE_RENDERING_PIPELINES_I_SPIRTE_PIPELINE_HPP
#define SFA_SRC_ENGINE_RENDERING_PIPELINES_I_SPIRTE_PIPELINE_HPP

#include "rendering/RenderCommand.hpp"

namespace sfa
{

class ISpritePipeline
{
public:
    ISpritePipeline() = default;
    virtual ~ISpritePipeline() = default;

    ISpritePipeline(const ISpritePipeline&) = delete;
    ISpritePipeline& operator=(const ISpritePipeline&) = delete;
    ISpritePipeline(ISpritePipeline&&) noexcept = delete;
    ISpritePipeline& operator=(ISpritePipeline&&) noexcept = delete;

    virtual void draw(const SpriteSubmission& submission) const = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_PIPELINES_I_SPIRTE_PIPELINE_HPP

