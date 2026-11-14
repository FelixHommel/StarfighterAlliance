#ifndef SFA_SRC_ENGINE_CORE_TEXTURE_VIEW_HPP
#define SFA_SRC_ENGINE_CORE_TEXTURE_VIEW_HPP

#include <glad/gl.h>

namespace sfa
{

/// \brief Non-owning reference to a \ref Texture2D.
///
/// \author Felix Hommel
/// \date 11/11/2026
struct TextureView
{
    GLuint id{ 0 };
    GLenum target{};
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CORE_TEXTURE_VIEW_HPP

