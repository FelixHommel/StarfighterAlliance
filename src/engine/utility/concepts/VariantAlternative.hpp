#ifndef SFA_SRC_ENGINE_UTILITY_CONCEPTS_VARIANT_ALTERNATIVE_HPP
#define SFA_SRC_ENGINE_UTILITY_CONCEPTS_VARIANT_ALTERNATIVE_HPP

#include <concepts>
#include <type_traits>
#include <variant>

namespace sfa::concepts
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
template<typename T, typename Variant> concept VariantAlternative = detail::IsVariantAlternative<T, Variant>::value;

} // namespace sfa::concepts

#endif // !SFA_SRC_ENGINE_UTILITY_CONCEPTS_VARIANT_ALTERNATIVE_HPP

