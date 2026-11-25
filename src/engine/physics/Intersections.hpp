#ifndef SFA_SRC_ENGINE_PHYSICS_INTERSECTIONS_HPP
#define SFA_SRC_ENGINE_PHYSICS_INTERSECTIONS_HPP

#include "physics/Shape.hpp"

namespace sfa::physics
{

/// \brief Check for intersection of two physical shapes.
///
/// \param lhs The first object involved in the collision check
/// \param rhs The second object involved in the collision check
///
/// \returns **true** if \p lhs and \p rhs are intersecting, **false** otherwise
[[nodiscard]] bool intersects(const Shape& lhs, const Shape& rhs);

} // namespace sfa::physics

#endif // !SFA_SRC_ENGINE_PHYSICS_INTERSECTIONS_HPP

