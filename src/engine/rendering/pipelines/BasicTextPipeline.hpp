#ifndef SFA_SRC_ENGINE_RENDERING_PIPELINES_BASIC_TEXT_PIPELINE_HPP
#define SFA_SRC_ENGINE_RENDERING_PIPELINES_BASIC_TEXT_PIPELINE_HPP

#include "core/Shader.hpp"
#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/ITextPipeline.hpp"

#include <glm/glm.hpp>

#include <memory>

namespace sfa
{

/// \brief Abstraction for rendering Text on the screen.
///
/// Rendering text is a quite difficult task, therefore this class abstracts it in a way where
/// only the text and position has to be supplied to render it to the screen.
///
/// \author Felix Hommel
/// \date 11/20/2026
class BasicTextPipeline final : public ITextPipeline
{
public:
    explicit BasicTextPipeline(std::shared_ptr<Shader> shader);
    ~BasicTextPipeline() override;

    BasicTextPipeline(const BasicTextPipeline&) = delete;
    BasicTextPipeline& operator=(const BasicTextPipeline&) = delete;
    BasicTextPipeline(BasicTextPipeline&&) noexcept = delete;
    BasicTextPipeline& operator=(BasicTextPipeline&&) noexcept = delete;

    /// \brief Begin the drawing of the next frame.
    ///
    /// \param projection The projection matrix
    void beginFrame(const glm::mat4& projection) const override;
    void endFrame() const override {}

    /// \brief Render text to the screen.
    ///
    /// \param submission The \ref TextSubmission containing the data
    void draw(const TextSubmission& submission) const override;

private:
    std::shared_ptr<Shader> m_shader;
    unsigned int m_vao{ 0 };
    unsigned int m_vbo{ 0 };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_PIPELINES_BASIC_TEXT_PIPELINE_HPP

