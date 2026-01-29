#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_BUTTON_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_BUTTON_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include <functional>

#include <glm/glm.hpp>

namespace sfa
{

/// \brief Give an entity button properties.
///
/// \author Felix Hommel
/// \date 1/29/2026
struct ButtonComponent : public IComponent
{
    glm::vec3 standardColor;
    float hoverFactor;
    float pressFactor;

    std::function<void()> onClick;
    float pressCooldownMax;
    float cooldownTimer{ 0.f };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_BUTTON_COMPONENT_HPP

