#ifndef SFA_SRC_ENGINE_CORE_TEXTURE_HPP
#define SFA_SRC_ENGINE_CORE_TEXTURE_HPP

#include "glad/gl.h"

#include <cstddef>
#include <span>

namespace sfa
{

/// \brief Abstracts OpenGL Textures.
///
/// Holds all the information of a Texture and registers it within OpenGL.
///
/// \author Felix Hommel
/// \date 11/17/2024
class Texture2D
{
public:
    /// \brief Create a new \ref Texture2D and set it up with OpenGL.
    ///
    /// \param width the width of the texture
    /// \param height the height of the texture
    /// \param channels the amount of channels that the texture has
    /// \param pixels the image data
    Texture2D(int width, int height, int channels, std::span<const std::byte> pixels);
    ~Texture2D();

    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    Texture2D(const Texture2D&) = delete("Textures should not be copy constructed because texture IDs should be unique");
    Texture2D& operator=(const Texture2D&) = delete("Textures should not be copy assigned because texture IDs should be unique");

    /// \brief Bind the texture to the OpenGL state.
    void bind() const;

    [[nodiscard]] unsigned int getID() const noexcept { return m_id; }
    [[nodiscard]] int width() const noexcept { return m_width; }
    [[nodiscard]] int height() const noexcept { return m_height; }

    /// \brief Configure the texture as an RGBA texture.
    void setRGBA();

private:
    static constexpr auto RGBA_CHANNELS{ 4 };

    unsigned int m_id{ 0 };
    int m_width{ 0 };
    int m_height{ 0 };
    int m_internalFormat{ GL_RGB };
    int m_imageFormat{ GL_RGB };
    int m_wrapS{ GL_REPEAT };
    int m_wrapT{ GL_REPEAT };
    int m_filterMin{ GL_REPEAT };
    int m_filterMax{ GL_REPEAT };

    void releaseTexture() const;
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_CORE_TEXTURE_HPP

