#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_UI_LAYOUT_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_UI_LAYOUT_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include <glm/glm.hpp>

#include <cstdint>

namespace sfa
{

/// \brief Give Layout properties to an entity.
///
/// \author Felix Hommel
/// \date 1/29/2026
struct UILayoutComponent : public IComponent
{
    enum class Type : std::uint8_t
    {
        Vertical,
        Horizontal
    };

    Type type;
    float spacing;
    glm::vec2 padding;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_UI_LAYOUT_COMPONENT_HPP

