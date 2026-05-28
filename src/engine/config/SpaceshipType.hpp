#ifndef SFA_SRC_ENGINE_CONFIG_SPACESHIP_TYPE_HPP
#define SFA_SRC_ENGINE_CONFIG_SPACESHIP_TYPE_HPP

#include <cstdint>

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

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CONFIG_SPACESHIP_TYPE_HPP
