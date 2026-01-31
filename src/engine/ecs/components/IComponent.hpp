#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_I_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_I_COMPONENT_HPP

#include <type_traits>

namespace sfa
{

/// \brief Interface for all components.
///
/// This allows for type safe dealing with Components.
struct IComponent
{};

/// \brief Concept defining what a valid Component is.
template<typename T> concept Component = std::is_base_of_v<IComponent, T>;

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_I_COMPONENT_HPP

