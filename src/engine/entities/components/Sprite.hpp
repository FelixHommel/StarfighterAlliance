#ifndef SFA_SRC_ENGINE_ENTITIES_COMPONENTS_SPRITE_HPP
#define SFA_SRC_ENGINE_ENTITIES_COMPONENTS_SPRITE_HPP

#include "entities/components/Transform.hpp"
#include "rendering/SpriteAppearance.hpp"

#include <glm/glm.hpp>

namespace sfa
{

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

