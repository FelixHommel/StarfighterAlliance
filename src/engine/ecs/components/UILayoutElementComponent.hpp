#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_UI_LAYOUT_ELEMENT_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_UI_LAYOUT_ELEMENT_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include <glm/glm.hpp>

namespace sfa
{

/// \brief Component to help UI elements that are part of a layout with sizing.
///
/// \author Felix Hommel
/// \date 2/24/2026
struct UILayoutElementComponent : public IComponent
{
    glm::vec2 prefferedSize;
    float flexGrow{ 0.f };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_UI_LAYOUT_ELEMENT_COMPONENT_HPP

