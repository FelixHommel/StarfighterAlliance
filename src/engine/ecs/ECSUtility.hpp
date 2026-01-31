#ifndef SFA_SRC_ENGINE_ECS_UTILITY_HPP
#define SFA_SRC_ENGINE_ECS_UTILITY_HPP

#include <cstddef>
#include <cstdint>

/// \brief Declare and set up the foundation for the ECS.
///
/// \author Felix Hommel
/// \date 1/25/2026
namespace sfa
{

using EntityID = std::uint32_t;      ///< Define what represents an EntityID
constexpr EntityID NULL_ENTITY{ 0 }; ///< Define what value represents a null/invalid entity

using ComponentTypeID = std::size_t; ///< Define what represents a ComponentTypeID

namespace detail
{

/// \brief Return the next component type ID in line.
///
/// \returns \ref ComponentTypeID
inline ComponentTypeID getNextComponentTypeID()
{
    // TODO: Implement a robuster system that also can reclaim released IDs to give the other components.
    static ComponentTypeID next{ 0 };
    return next++;
}

} // namespace detail

/// \brief Retrieve the ID of a component type.
///
/// Use static and template trickery to save the IDs for components.
///
/// \tparam T type of the component
///
/// \returns \ref ComponentTypeID of \p T
template<typename T>
ComponentTypeID getComponentTypeID()
{
    static ComponentTypeID typeID{ detail::getNextComponentTypeID() };
    return typeID;
}

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_UTILITY_HPP

