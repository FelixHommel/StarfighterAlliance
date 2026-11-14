#ifndef SFA_SRC_ENGINE_CONFIG_SPACESHIP_COLOR_HPP
#define SFA_SRC_ENGINE_CONFIG_SPACESHIP_COLOR_HPP

#include <cstdint>
#include <format>
#include <string>

namespace sfa
{

/// \brief Possible of spaceship colors
///
/// \author Felix Hommel
/// \date 5/24/2026
enum class SpaceshipColor : std::uint8_t
{
    Red,
    Blue,
    Yellow,
    Orange,
    Green,
    Violet,
    White,
    Black,
    Transparent
};

namespace spaceshipColor
{

[[nodiscard]] constexpr std::string toString(SpaceshipColor color) noexcept
{
    switch(color)
    {
        using enum SpaceshipColor;
    case Red:
        return "red";
    case Blue:
        return "blue";
    case Yellow:
        return "yellow";
    case Orange:
        return "ornage";
    case Green:
        return "green";
    case Violet:
        return "violet";
    case White:
        return "white";
    case Black:
        return "black";
    case Transparent:
        return "transparent";
    }
}

} // namespace spaceshipColor

} // namespace sfa

template<>
struct std::formatter<sfa::SpaceshipColor> : public std::formatter<std::string>
{
    using Parent = std::formatter<std::string>;

    auto format(sfa::SpaceshipColor color, std::format_context& ctx) const
    {
        return Parent::format(sfa::spaceshipColor::toString(color), ctx);
    }
};

#endif // !SFA_SRC_ENGINE_CONFIG_SPACESHIP_COLOR_HPP
