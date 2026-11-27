#ifndef SFA_SRC_ENGINE_RENDERING_SPRITE_APPEARANCE_HPP
#define SFA_SRC_ENGINE_RENDERING_SPRITE_APPEARANCE_HPP

#include "core/TextureView.hpp"

#include <glm/glm.hpp>

#include <variant>

namespace sfa
{

using SpriteAppearance = std::variant<TextureView, glm::vec3>;

} // namespace sfa

#endif // !SFA_SRC_ENGINE_RENDERING_SPRITE_APPEARANCE_HPP

