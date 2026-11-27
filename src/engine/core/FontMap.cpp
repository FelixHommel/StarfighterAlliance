#include "FontMap.hpp"

#include <ft2build.h>
// NOLINTNEXTLINE(misc-include-cleaner): FT_FREETYPE_H is a macro that is created by FreeType. That is it's inteded use.
#include FT_FREETYPE_H
#include <freetype/fttypes.h>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <limits>
#include <string_view>
#include <utility>

namespace
{

constexpr auto ADVANCE_BITSHIFT{ 6 };

/// \brief Create a \ref sfa::FontMap::Character out of an FT_Face.
///
/// \param face The Ft_Face object that contains the character information
///
/// \returns a \ref sfa::FontMap::Character from the information of the face
sfa::FontMap::Character makeCharacter(const FT_Face& face)
{
    GLuint texture{ 0 };
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

    return {
        .textureID = texture,
        .size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        .bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        .advance = static_cast<unsigned int>(face->glyph->advance.x),
    };
}

} // namespace

namespace sfa
{

FontMap::FontMap(const std::filesystem::path& filepath, unsigned int fontSize)
{
    constexpr std::size_t LOADED_ASCII_CHARS{ 128 };
    constexpr FT_Error FT_SUCCESS_CODE{ 0 };

    FT_Library ft{};
    if(FT_Init_FreeType(&ft) != FT_SUCCESS_CODE)
        spdlog::error("FREETYPE: Could not init FreeType Library");

    FT_Face face{};
    if(FT_New_Face(ft, reinterpret_cast<const char*>(filepath.u8string().c_str()), 0, &face) != FT_SUCCESS_CODE)
        spdlog::error("FREETYPE: Failed to load font from: {}", filepath.string());

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(GLubyte c{ 0 }; c < LOADED_ASCII_CHARS; ++c)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER) != FT_SUCCESS_CODE)
        {
            spdlog::error("FREETYTPE: Failed to load Glyph for '{}'", c);
            continue;
        }

        m_characters.emplace(static_cast<unsigned char>(c), ::makeCharacter(face));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

FontMap::~FontMap()
{
    releaseGLHandles();
}

FontMap::FontMap(FontMap&& other) noexcept : m_characters{ std::exchange(other.m_characters, {}) } {}
FontMap& FontMap::operator=(FontMap&& other) noexcept
{
    if(this == &other)
        return *this;

    releaseGLHandles();

    m_characters = std::exchange(other.m_characters, {});

    return *this;
}

FontMap::TextBounds FontMap::measureBounds(std::string_view text, const glm::vec2& scale) const
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

    for(const auto& c : text)
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

            penX += static_cast<float>(ch.advance >> ::ADVANCE_BITSHIFT) * scale.x;
        }
    }

    if(minX > maxX || minY > maxY)
        return {};

    return { .min = glm::vec2(minX, minY), .size = glm::vec2(maxX - minX, maxY - minY) };
}

glm::vec2 FontMap::measure(std::string_view text, const glm::vec2& scale) const
{
    return measureBounds(text, scale).size;
}

void FontMap::releaseGLHandles()
{
    for(const auto& c : m_characters)
        glDeleteTextures(1, &c.second.textureID);
}

} // namespace sfa

