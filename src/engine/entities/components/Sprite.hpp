#ifndef SFA_SRC_ENGINE_ENTITIES_COMPONENTS_SPRITE_HPP
#define SFA_SRC_ENGINE_ENTITIES_COMPONENTS_SPRITE_HPP

#include "core/TextureView.hpp"
#include "entities/components/Transform.hpp"

#include <glm/glm.hpp>

#include <variant>

namespace sfa
{

using SpriteAppearance = std::variant<TextureView, glm::vec3>;

/// \brief Sprite component of an entity.
///
/// \author Felix Hommel
/// \date 11/18/2026
struct Sprite
{
    SpriteAppearance appearance;
    Transform transform;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ENTITIES_COMPONENTS_SPRITE_HPP

