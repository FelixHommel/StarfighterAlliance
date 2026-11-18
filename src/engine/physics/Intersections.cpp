#include "Intersections.hpp"

#include "physics/Shape.hpp"

#include <concepts>
#include <type_traits>
#include <variant>

namespace
{

namespace detail
{

/// \brief Struct to help determine whether a \ref std::variant has a type for an alternative.
template<typename T, typename Variant>
struct IsVariantAlternative;

/// \brief Struct to help determine whether a \ref std::variant has a type for an alternative.
template<typename T, typename... Ts>
struct IsVariantAlternative<T, std::variant<Ts...>> : std::bool_constant<(std::same_as<T, Ts> || ...)>
{};

} // namespace detail

/// \brief A concept to ensure that \p Variant contains \p T as an alternative.
template<typename T, typename Variant> concept VariantAlternative = ::detail::IsVariantAlternative<T, Variant>::value;

/// \brief Utility overload to save redundancy in overload functions to check for collision between two \ref Shape.
///
/// This saves having to implement intersectsImpl(A, B) and intersectsImpl(B, A). Only one of them needs to be implemented.
///
/// \tparam T The first shape type
/// \tparam Q The second shape type
///
/// \param lhs The first object that is checked for intersection
/// \param rhs The second object that is checked for intersection
///
/// \returns **true** if \p lhs and \p rhs intersect, **false** otherwise
template<::VariantAlternative<sfa::physics::Shape> T, ::VariantAlternative<sfa::physics::Shape> Q>
    requires(!std::is_same_v<T, Q>)
bool intersectsImpl(const T& lhs, const Q& rhs)
{
    return ::intersectsImpl(rhs, lhs);
}

// NOLINTBEGIN(misc-unused-parameters): not implemented yet

/// \brief Intersection between two \ref physics::AABB
///
/// \param lhs The first \ref physics::AABB that is checked for intersection
/// \param rhs The second \ref physics::AABB that is checked for intersection
///
/// \returns **true** if \p lhs and \p rhs intersect, **false** otherwise
bool intersectsImpl(const sfa::physics::AABB& lhs, const sfa::physics::AABB& rhs)
{
    return false; // TODO: Implement AABB - AABB collision
}

/// \brief Intersection between a \ref physics::AABB and a \ref physics::Circle
///
/// \param lhs The first \ref physics::AABB that is checked for intersection
/// \param rhs The second \ref physics::Circle that is checked for intersection
///
/// \returns **true** if \p lhs and \p rhs intersect, **false** otherwise
bool intersectsImpl(const sfa::physics::AABB& lhs, const sfa::physics::Circle& rhs)
{
    return false; // TODO: Implement AABB - Circle collision
}

/// \brief Intersection between a \ref physics::Circle and a \ref physics::Circle
///
/// \param lhs The first \ref physics::Circle that is checked for intersection
/// \param rhs The second \ref physics::Circle that is checked for intersection
///
/// \returns **true** if \p lhs and \p rhs intersect, **false** otherwise
bool intersectsImpl(const sfa::physics::Circle& lhs, const sfa::physics::Circle& rhs)
{
    return false; // TODO: Implement Circle - Circle collision
}

// NOLINTEND(misc-unused-parameters)

} // namespace

namespace sfa::physics
{

bool intersects(const Shape& lhs, const Shape& rhs)
{
    return std::visit([](const auto& lhs, const auto& rhs) -> bool { return ::intersectsImpl(lhs, rhs); }, lhs, rhs);
}

} // namespace sfa::physics

