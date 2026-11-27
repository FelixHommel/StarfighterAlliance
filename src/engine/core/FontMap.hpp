#ifndef SFA_SRC_ENGINE_CORE_FONT_HPP
#define SFA_SRC_ENGINE_CORE_FONT_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <map>
#include <string_view>

namespace sfa
{

class FontMap
{
public:
    /// \brief Representation of a single character.
    ///
    /// \author Felix Hommel
    /// \date 11/17/2026
    struct Character
    {
        GLuint textureID;     ///< ID handle of the glyph texture
        glm::ivec2 size;      ///< size of glyph
        glm::ivec2 bearing;   ///< offset from baseline to left/top of glyph
        unsigned int advance; ///< horizontal offset to advance to next glyph
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

    /// \brief Load a font from a file.
    ///
    /// \param filepath path to the font. Should be an .fft file
    /// \param fontSize (optional) the size the font will be
    FontMap(const std::filesystem::path& filepath, unsigned int fontSize = DEFAULT_FONT_SIZE);
    ~FontMap();

    FontMap(const FontMap&) = delete;
    FontMap& operator=(const FontMap&) = delete;
    FontMap(FontMap&& other) noexcept;
    FontMap& operator=(FontMap&& other) noexcept;

    [[nodiscard]] const Character& at(unsigned char c) const noexcept { return m_characters.at(c); }

    /// \brief Estimate bounds of rendered text for the current loaded font.
    ///
    /// \param text the string that is measured
    /// \param scale the scale of the text
    [[nodiscard]] TextBounds measureBounds(std::string_view text, const glm::vec2& scale = DEFAULT_SCALE) const;
    /// \brief Estimate the rendered dimensions of a text string for the currently loaded font.
    ///
    /// \param text the string that is measured
    /// \param scale the scale of the text
    [[nodiscard]] glm::vec2 measure(std::string_view text, const glm::vec2& scale = DEFAULT_SCALE) const;

private:
    static constexpr auto DEFAULT_FONT_SIZE{ 24 };
    static constexpr auto DEFAULT_SCALE{ glm::vec2(1.f) };

    std::map<unsigned char, Character> m_characters;

    void releaseGLHandles();
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CORE_FONT_HPP

