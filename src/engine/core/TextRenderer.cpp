#include "TextRenderer.hpp"

#include "Shader.hpp"

#include "ft2build.h"
#include <filesystem>
#include <limits>
// NOLINTNEXTLINE(misc-include-cleaner): FT_FREETYPE_H is a macro that is created by FreeType. That is it's inteded use.
#include FT_FREETYPE_H

#include "glad/gl.h"
#include "glm/ext/matrix_clip_space.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace sfa
{

TextRenderer::TextRenderer(std::shared_ptr<Shader> shader) : m_shader(std::move(shader))
{
    m_shader->setInteger("text", 0);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GLYPH_VERTICES * GLYPH_VERTEX_ATTRIBUTES, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        0, GLYPH_VERTEX_ATTRIBUTES, GL_FLOAT, GL_FALSE, GLYPH_VERTEX_ATTRIBUTES * sizeof(float), nullptr
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void TextRenderer::load(const std::filesystem::path& filepath, unsigned int fontSize)
{
    m_characters.clear();

    FT_Library ft{};
    if(FT_Init_FreeType(&ft) != 0)
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face{};
    const auto u8path{ filepath.u8string() };
    if(FT_New_Face(ft, reinterpret_cast<const char*>(u8path.c_str()), 0, &face) != 0)
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // NOTE: Load Glyph for the first 128 ASCII Characters
    for(GLubyte c{ 0 }; c < LOADED_ASCII_CHARS; ++c)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        unsigned int texture{ 0 };
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            static_cast<int>(face->glyph->bitmap.width),
            static_cast<int>(face->glyph->bitmap.rows),
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character{ .textureID = texture,
                             .size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                             .bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                             .advance = static_cast<unsigned int>(face->glyph->advance.x) };
        m_characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::beginFrame(const glm::mat4& projection)
{
    m_shader->setMatrix4("projection", projection, true);
}

void TextRenderer::render(const std::string& text, const glm::vec2& pos, const glm::vec2& scale, glm::vec3 color)
{
    m_shader->setVector3f("textColor", color, true);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_vao);

    float x{ pos.x };
    for(auto c{ text.cbegin() }; c != text.cend(); c++)
    {
        const Character ch{ m_characters[*c] };

        const float xpos{ x + (static_cast<float>(ch.bearing.x) * scale.x) };
        const float ypos{ pos.y + (static_cast<float>(m_characters['H'].bearing.y - ch.bearing.y) * scale.y) };

        const float w{ static_cast<float>(ch.size.x) * scale.x };
        const float h{ static_cast<float>(ch.size.y) * scale.y };
        std::array<float, GLYPH_VERTICES * GLYPH_VERTEX_ATTRIBUTES> vertices{
            xpos, ypos + h, 0.0f, 1.0f, xpos + w, ypos,     1.0f, 0.0f, xpos,     ypos, 0.0f, 0.0f,
            xpos, ypos + h, 0.0f, 1.0f, xpos + w, ypos + h, 1.0f, 1.0f, xpos + w, ypos, 1.0f, 0.0f
        };
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, GLYPH_VERTICES);

        // NOTE: Bitshift by 6 == 2^6, advance is 1/64 of a pixel
        x += static_cast<float>(ch.advance >> ADVANCE_BITSHIFT) * scale.x;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

TextBounds TextRenderer::measureBounds(const std::string& text, const glm::vec2& scale) const
{
    if(text.empty() || m_characters.empty())
        return {};

    const auto hIt{ m_characters.find('H') };
    const float hBearing{ hIt != m_characters.end() ? static_cast<float>(hIt->second.bearing.y) : 0.f };

    float penX{ 0.f };
    float minX{ std::numeric_limits<float>::max() };
    float minY{ std::numeric_limits<float>::max() };
    float maxX{ std::numeric_limits<float>::lowest() };
    float maxY{ std::numeric_limits<float>::lowest() };

    for(const auto c : text)
    {
        if(const auto it{ m_characters.find(c) }; it != m_characters.end())
        {
            const auto& ch{ it->second };

            const float xpos{ penX + (static_cast<float>(ch.bearing.x) * scale.x) };
            const float ypos{ (hBearing - static_cast<float>(ch.bearing.y)) * scale.y };
            const float w{ static_cast<float>(ch.size.x) * scale.x };
            const float h{ static_cast<float>(ch.size.y) * scale.y };

            minX = std::min(minX, xpos);
            minY = std::min(minY, ypos);
            maxX = std::max(maxX, xpos + w);
            maxY = std::max(maxY, ypos + h);

            penX += static_cast<float>(ch.advance >> ADVANCE_BITSHIFT) * scale.x;
        }
    }

    if(minX > maxX || minY > maxY)
        return {};

    return { .min = { minX, minY }, .size = { maxX - minX, maxY - minY } };
}

glm::vec2 TextRenderer::measure(const std::string& text, const glm::vec2& scale) const
{
    return measureBounds(text, scale).size;
}

} // namespace sfa

