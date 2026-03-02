#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_UI_TRANSFORM_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_UI_TRANSFORM_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include <glm/glm.hpp>

namespace sfa
{

/// \brief UI element specific transform component allowing for parent dependent positioning.
///
/// \author Felix Hommel
/// \date 2/24/2026
struct UITransformComponent : public IComponent
{
    glm::vec2 localPosition;
    glm::vec2 worldPosition;
    glm::vec2 size;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_UI_TRANSFORM_COMPONENT_HPP

