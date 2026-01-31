#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_LAYOUT_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_LAYOUT_COMPONENT_HPP

#include "ecs/ECSUtility.hpp"
#include "ecs/components/IComponent.hpp"

#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

namespace sfa
{

/// \brief Give Layout properties to an entity.
///
/// \author Felix Hommel
/// \date 1/29/2026
struct LayoutComponent : public IComponent
{
    enum class Type : std::uint8_t
    {
        Vertical,
        Horizontal,
        Grid
    };

    Type type;
    float spacing;
    glm::vec2 padding;

    std::vector<EntityID> children;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_LAYOUT_COMPONENT_HPP

