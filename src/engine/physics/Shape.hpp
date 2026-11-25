#ifndef SFA_SRC_ENGINE_PHYSICS_SHAPE_HPP
#define SFA_SRC_ENGINE_PHYSICS_SHAPE_HPP

#include <glm/glm.hpp>

#include <variant>

namespace sfa::physics
{

/// \brief Axis-aligned bounding box for Rectangular collision behavior.
///
/// \author Felix Hommel
/// \date 11/16/2026
struct AABB
{
    glm::vec2 min;
    glm::vec2 max;
};

/// \brief Circular collision behavior.
///
/// \author Felix Hommel
/// \date 11/16/2026
struct Circle
{
    glm::vec2 center;
    float radius;
};

/// \brief Physical collision shape.
using Shape = std::variant<AABB, Circle>;

} // namespace sfa::physics

#endif // !SFA_SRC_ENGINE_PHYSICS_SHAPE_HPP

