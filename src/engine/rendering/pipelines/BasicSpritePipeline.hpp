#ifndef SFA_SRC_ENGINE_RENDERING_PIPELINES_SPRITE_PIPELINE_HPP
#define SFA_SRC_ENGINE_RENDERING_PIPELINES_SPRITE_PIPELINE_HPP

#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/ISpritePipeline.hpp"

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace sfa
{

class BasicSpritePipeline : public ISpritePipeline
{
public:
    BasicSpritePipeline();
    ~BasicSpritePipeline() override;

    BasicSpritePipeline(const BasicSpritePipeline&) = delete;
    BasicSpritePipeline& operator=(const BasicSpritePipeline&) = delete;
    BasicSpritePipeline(BasicSpritePipeline&&) noexcept = delete;
    BasicSpritePipeline& operator=(BasicSpritePipeline&&) noexcept = delete;

    /// \brief Draw a textured quad to the screen
    ///
    /// The order of actions within the function is important, especially the translation, rotation, and scale order.
    ///     1. translate - bring the quad to the correct position
    ///     2. rotate - rotate the quad to the correct angle
    ///     3. scale - scale the quad to the correct size
    ///
    /// Note that the actual coordinates of all the quads are the same. The position on the screen is computed with matrix
    /// transformations, which achieves the same visual results.
    ///
    /// \param submission \ref SpriteSubmission with the data of the sprite that is drawn
    void draw(const SpriteSubmission& submission) const override;

private:
    GLuint m_quadVAO{ 0 };
    GLuint m_fallbackTexture{ 0 };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_PIPELINES_SPRITE_PIPELINE_HPP

