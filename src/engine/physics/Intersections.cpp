#include "Intersections.hpp"

#include "physics/Shape.hpp"

#include <type_traits>
#include <variant>

namespace
{

template<typename T, typename Q>
    requires(!std::is_same_v<T, Q>)
bool intersectsImpl(const T& lhs, const Q& rhs)
{
    return ::intersectsImpl(rhs, lhs);
}

bool intersectsImpl(const sfa::physics::AABB& lhs, const sfa::physics::AABB& rhs)
{
    return false; // TODO: Implement AABB - AABB collision
}

bool intersectsImpl(const sfa::physics::AABB& lhs, const sfa::physics::Circle& rhs)
{
    return false; // TODO: Implement AABB - Circle collision
}

bool intersectsImpl(const sfa::physics::Circle& lhs, const sfa::physics::Circle& rhs)
{
    return false; // TODO: Implement Circle - Circle collision
}

} // namespace

namespace sfa::physics
{

bool intersects(const Shape& lhs, const Shape& rhs)
{
    return std::visit([](const auto& lhs, const auto& rhs) -> bool { return ::intersectsImpl(lhs, rhs); }, lhs, rhs);
}

} // namespace sfa::physics

