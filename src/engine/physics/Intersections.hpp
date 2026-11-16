#ifndef SFA_SRC_ENGINE_PHYSICS_INTERSECTIONS_HPP
#define SFA_SRC_ENGINE_PHYSICS_INTERSECTIONS_HPP

#include "physics/Shape.hpp"

namespace sfa::physics
{

bool intersects(const Shape& lhs, const Shape& rhs);

} // namespace sfa::physics

#endif // !SFA_SRC_ENGINE_PHYSICS_INTERSECTIONS_HPP

