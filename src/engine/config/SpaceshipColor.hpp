#ifndef SFA_SRC_ENGINE_CONFIG_SPACESHIP_COLOR_HPP
#define SFA_SRC_ENGINE_CONFIG_SPACESHIP_COLOR_HPP

#include <cstdint>

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

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CONFIG_SPACESHIP_COLOR_HPP
