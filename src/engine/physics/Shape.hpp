#ifndef SFA_SRC_ENGINE_PHYSICS_SHAPE_HPP
#define SFA_SRC_ENGINE_PHYSICS_SHAPE_HPP

#include <glm/glm.hpp>

#include <variant>

namespace sfa::physics
{

struct AABB
{
    glm::vec2 min;
    glm::vec2 max;
};

struct Circle
{
    glm::vec2 center;
    float radius;
};

using Shape = std::variant<AABB, Circle>;

} // namespace sfa::physics

#endif // !SFA_SRC_ENGINE_PHYSICS_SHAPE_HPP

