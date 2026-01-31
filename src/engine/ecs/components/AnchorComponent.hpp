#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_ANCHOR_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_ANCHOR_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include <cstdint>

#include <glm/glm.hpp>

namespace sfa
{

/// \brief Give Anchor properties to an entity.
///
/// \author Felix Hommel
/// \date 1/29/2026
struct AnchorComponent : public IComponent
{
    enum class Point : std::uint8_t
    {
        TopLeft,
        TopCenter,
        TopRoght,
        CenterLeft,
        Center,
        CenterRight,
        BottomLeft,
        BottomCenter,
        BottomRight
    };

    Point point;
    glm::vec2 offset;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_ANCHOR_COMPONENT_HPP

