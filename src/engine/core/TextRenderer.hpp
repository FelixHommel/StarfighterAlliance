#ifndef SFA_SRC_ENGINE_CORE_TEXT_RENDERER_HPP
#define SFA_SRC_ENGINE_CORE_TEXT_RENDERER_HPP

#include "Shader.hpp"

#include <glm/glm.hpp>

#include <cstddef>
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

/// \brief Abstraction for rendering Text on the screen.
///
/// Rendering text is a quite difficult task, therefore this class abstracts it in a way where
/// only the text and position has to be supplied to render it to the screen.
///
/// \author Felix Hommel
/// \date 11/17/2024
class TextRenderer
{
public:
    explicit TextRenderer(std::shared_ptr<Shader> shader);
    ~TextRenderer();

    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;
    TextRenderer(TextRenderer&&) = delete;
    TextRenderer& operator=(TextRenderer&&) = delete;

    /// \brief Load a font from a file.
    ///
    /// \param filepath path to the font. Should be an .fft file
    /// \param fontSize(optional) the size the font will be
    void load(const std::filesystem::path& filepath, unsigned int fontSize = DEFAULT_FONT_SIZE);

    /// \brief Begin the drawing of the next frame.
    ///
    /// \param projection projection matrix
    void beginFrame(const glm::mat4& projection);

    /// \brief Render text to the screen.
    ///
    /// \param text the text that will be drawn.
    /// \param pos the position of the text
    /// \param scale(optional) apply extra scale to the text
    /// \param color(optional) the color of the text
    void render(const std::string& text, const glm::vec2& pos, const glm::vec2& scale = DEFAULT_SCALE, glm::vec3 color = DEFAULT_COLOR);

private:
    static constexpr std::size_t LOADED_ASCII_CHARS{ 128 };
    static constexpr std::size_t GLYPH_VERTICES{ 6 };
    static constexpr std::size_t GLYPH_VERTEX_ATTRIBUTES{ 4 };
    static constexpr auto ADVANCE_BITSHIFT{ 6 };

    static constexpr auto DEFAULT_FONT_SIZE{ 24 };
    static constexpr auto DEFAULT_SCALE{ glm::vec2(1.f) };
    static constexpr auto DEFAULT_COLOR{ glm::vec3(1.f) };

    std::shared_ptr<Shader> m_shader;
    unsigned int m_vao{ 0 };
    unsigned int m_vbo{ 0 };

    std::map<char, Character> m_characters;
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_CORE_TEXT_RENDERER_HPP

