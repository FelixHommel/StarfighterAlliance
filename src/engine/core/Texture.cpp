#include "Texture.hpp"

#include <glad/gl.h>

#include <cstddef>
#include <span>
#include <utility>

namespace sfa
{

Texture2D::Texture2D(int width, int height, int channels, std::span<const std::byte> pixels)
    : m_width{ width }
    , m_height{ height }
{
    if(channels == RGBA_CHANNELS)
        setRGBA();

    glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        m_internalFormat,
        m_width,
        m_height,
        0,
        m_imageFormat,
        GL_UNSIGNED_BYTE,
        static_cast<const void*>(pixels.data())
    );

    // NOTE: Maybe GL_UNPACK_ALIGNMENT needs to be configured with glPixelStorei(...)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMax);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
    releaseTexture();
}

Texture2D::Texture2D(Texture2D&& other) noexcept
    : m_id(std::exchange(other.m_id, 0))
    , m_width(std::exchange(other.m_width, 0))
    , m_height(std::exchange(other.m_height, 0))
    , m_internalFormat(std::exchange(other.m_internalFormat, GL_RGB))
    , m_imageFormat(std::exchange(other.m_imageFormat, GL_RGB))
    , m_wrapS(std::exchange(other.m_wrapS, GL_REPEAT))
    , m_wrapT(std::exchange(other.m_wrapT, GL_REPEAT))
    , m_filterMin(std::exchange(other.m_filterMin, GL_LINEAR))
    , m_filterMax(std::exchange(other.m_filterMax, GL_LINEAR))
{}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    if(this == &other)
        return *this;

    releaseTexture();

    m_id = std::exchange(other.m_id, 0);
    m_width = std::exchange(other.m_width, 0);
    m_height = std::exchange(other.m_height, 0);
    m_internalFormat = std::exchange(other.m_internalFormat, GL_RGB);
    m_imageFormat = std::exchange(other.m_imageFormat, GL_RGB);
    m_wrapS = std::exchange(other.m_wrapS, GL_REPEAT);
    m_wrapT = std::exchange(other.m_wrapT, GL_REPEAT);
    m_filterMin = std::exchange(other.m_filterMin, GL_LINEAR);
    m_filterMax = std::exchange(other.m_filterMax, GL_LINEAR);

    return *this;
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::setRGBA()
{
    m_imageFormat = GL_RGBA;
    m_internalFormat = GL_RGBA;
}

/// \brief If the \ref Texture is holding a valid texture ID delete it.
void Texture2D::releaseTexture() const
{
    if(m_id != 0)
        glDeleteTextures(1, &m_id);
}

} // namespace sfa

