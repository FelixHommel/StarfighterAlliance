#ifndef SFA_SRC_ENGINE_CORE_TEXT_RENDERER_HPP
#define SFA_SRC_ENGINE_CORE_TEXT_RENDERER_HPP

#include "Shader.hpp"
#include "ResourceManager.hpp"

#include "glm/ext/vector_int2.hpp"

#include <cstddef>
#include <map>
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
    TextRenderer(unsigned int width, unsigned int height);
    ~TextRenderer();

    TextRenderer(const TextRenderer&) = delete;
    TextRenderer(TextRenderer&&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;
    TextRenderer& operator=(TextRenderer&&) = delete;

    void load(std::string font, unsigned int fontSize = DEFAULT_FONT_SIZE);
    void draw(std::string text, float x, float y, float scale = 1.f, glm::vec3 color = glm::vec3(1.f));

private:
    static constexpr std::size_t LOADED_ASCII_CHARS{ 128 };
    static constexpr auto DEFAULT_FONT_SIZE{ 24 };
    static constexpr std::size_t GLYPH_VERTICES{ 6 };
    static constexpr std::size_t GLYPH_VERTEX_ATTRIBUTES{ 4 };
    static constexpr auto ADVANCE_BITSHIFT{ 6 };

    unsigned int m_vao{ 0 };
    unsigned int m_vbo{ 0 };
    Shader m_textShader{ ResourceManager::loadShader("resources/shaders/text.vert", "resources/shaders/text.frag", nullptr, "text") };

    std::map<char, Character> m_characters;
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_CORE_TEXT_RENDERER_HPP

