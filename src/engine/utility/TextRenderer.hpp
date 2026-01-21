#ifndef INTERNAL_TEXT_RENDERER_HPP
#define INTERNAL_TEXT_RENDERER_HPP

#include "Shader.hpp"

#include "glm/ext/vector_int2.hpp"

#include <map>

struct Character
{
    unsigned int textureID; // ID handle of the glyph texture
    glm::ivec2 size;        // size of glyph
    glm::ivec2 bearing;     // offset from baseline to left/top of glyph
    unsigned int advance;   // horizontal offset to advance to next glyph
};

/*
 * @file TextRenderer.hpp
 * @brief Abstraction for rendering Text on the screen
 *
 * @details Rendering text is a quite difficult task, therefore this class abstracts it in a way where
 *          only the text and position has to be supplied to render it to the screen
 *
 * @author Felix Hommel
 * @date Nov 17, 2024
 */
class TextRenderer
{
public:
    /** Constructor / Destructor */
    TextRenderer(unsigned int width, unsigned int height);
    ~TextRenderer();

    /** Delete move constructor / assignment operator and leave the copy constructor / assignmetn operator at default */
    TextRenderer(const TextRenderer&) = default;
    TextRenderer(TextRenderer&&) = delete;
    TextRenderer& operator=(const TextRenderer&) = default;
    TextRenderer& operator=(TextRenderer&&) = delete;

    /** Public member functions */
    void load(std::string font, unsigned int fontSize = DEFAULT_FONT_SIZE);
    void draw(std::string text, float x, float y, float scale = 1.f, glm::vec3 color = glm::vec3(1.0f));

private:
    /** Member variables */
    unsigned int m_vao;
    unsigned int m_vbo;
    Shader m_textShader;

    std::map<char, Character> m_characters;

    /** Default values */
    static constexpr auto DEFAULT_FONT_SIZE{ 24 };
};

#endif //! INTERNAL_TEXT_RENDERER_HPP
