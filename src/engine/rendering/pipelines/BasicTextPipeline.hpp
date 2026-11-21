#ifndef SFA_SRC_ENGINE_RENDERING_PIPELINES_BASIC_TEXT_PIPELINE_HPP
#define SFA_SRC_ENGINE_RENDERING_PIPELINES_BASIC_TEXT_PIPELINE_HPP

#include "core/Shader.hpp"
#include "rendering/RenderCommand.hpp"
#include "rendering/pipelines/ITextPipeline.hpp"

#include <glm/glm.hpp>

#include <filesystem>
#include <map>
#include <memory>
#include <string>

namespace sfa
{

/// \brief Representation of a single character.
///
/// \author Felix Hommel
/// \date 11/17/2026
struct Character
{
    unsigned int textureID; ///< ID handle of the glyph texture
    glm::ivec2 size;        ///< size of glyph
    glm::ivec2 bearing;     ///< offset from baseline to left/top of glyph
    unsigned int advance;   ///< horizontal offset to advance to next glyph
};

/// \brief Information about the dimensions and bounds of a string.
///
/// \author Felix Hommel
/// \date 3/2/2026
struct TextBounds
{
    glm::vec2 min{ 0.f };
    glm::vec2 size{ 0.f };
};

/// \brief Abstraction for rendering Text on the screen.
///
/// Rendering text is a quite difficult task, therefore this class abstracts it in a way where
/// only the text and position has to be supplied to render it to the screen.
///
/// \author Felix Hommel
/// \date 11/20/2026
class BasicTextPipeline : public ITextPipeline
{
public:
    explicit BasicTextPipeline(std::shared_ptr<Shader> shader);
    ~BasicTextPipeline() override;

    BasicTextPipeline(const BasicTextPipeline&) = delete;
    BasicTextPipeline& operator=(const BasicTextPipeline&) = delete;
    BasicTextPipeline(BasicTextPipeline&&) noexcept = delete;
    BasicTextPipeline& operator=(BasicTextPipeline&&) noexcept = delete;

    /// \brief Load a font from a file.
    ///
    /// \param filepath path to the font. Should be an .fft file
    /// \param fontSize (optional) the size the font will be
    void load(const std::filesystem::path& filepath, unsigned int fontSize = DEFAULT_FONT_SIZE);

    /// \brief Begin the drawing of the next frame.
    ///
    /// \param projection The projection matrix
    void beginFrame(const glm::mat4& projection) const override;
    void endFrame() const override {}

    /// \brief Render text to the screen.
    ///
    /// \param submission The \ref TextSubmission containing the data
    void draw(const TextSubmission& submission) const override;

    /// \brief Estimate bounds of rendered text for the current loaded font.
    ///
    /// \param text the string that is measured
    /// \param scale the scale of the text
    [[nodiscard]] TextBounds measureBounds(const std::string& text, const glm::vec2& scale = DEFAULT_SCALE) const;
    /// \brief Estimate the rendered dimensions of a text string for the currently loaded font.
    ///
    /// \param text the string that is measured
    /// \param scale the scale of the text
    [[nodiscard]] glm::vec2 measure(const std::string& text, const glm::vec2& scale = DEFAULT_SCALE) const;

private:
    static constexpr auto DEFAULT_FONT_SIZE{ 24 };
    static constexpr auto DEFAULT_SCALE{ glm::vec2(1.f) };

    std::shared_ptr<Shader> m_shader;
    unsigned int m_vao{ 0 };
    unsigned int m_vbo{ 0 };

    std::map<unsigned char, Character> m_characters;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_PIPELINES_BASIC_TEXT_PIPELINE_HPP

