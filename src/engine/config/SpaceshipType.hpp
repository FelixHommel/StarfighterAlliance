#ifndef SFA_SRC_ENGINE_CONFIG_SPACESHIP_TYPE_HPP
#define SFA_SRC_ENGINE_CONFIG_SPACESHIP_TYPE_HPP

#include <cstdint>
#include <format>
#include <string>

namespace sfa
{

/// \brief Possible of spaceship types
///
/// \author Felix Hommel
/// \date 5/24/2026
enum class SpaceshipType : std::uint8_t
{
    XWing = 0,
    YWing = 1,
    MilleniumFalcon = 2,
    SPACESHIP_TPYE_MAX = 3
};

namespace spaceshipType
{

[[nodiscard]] constexpr std::string toString(SpaceshipType type) noexcept
{
    switch(type)
    {
        using enum SpaceshipType;
    case XWing:
        return "x_wing";
    case YWing:
        return "y_wing";
    case MilleniumFalcon:
        return "millenium_falcon";
    default:
        return "unknown";
    }
}

} // namespace spaceshipType

} // namespace sfa

template<>
struct std::formatter<sfa::SpaceshipType> : public std::formatter<std::string>
{
    auto format(sfa::SpaceshipType type, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(sfa::spaceshipType::toString(type), ctx);
    }
};

#endif // !SFA_SRC_ENGINE_CONFIG_SPACESHIP_TYPE_HPP
