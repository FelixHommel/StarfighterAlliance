#ifndef SFA_SRC_ENGINE_SCREENS_GAME_SESSION_HPP
#define SFA_SRC_ENGINE_SCREENS_GAME_SESSION_HPP

#include "concurrency/AsyncValue.hpp"
#include "config/SpaceshipColor.hpp"
#include "config/SpaceshipType.hpp"

namespace
{

/// \brief Information about the spaceship
///
/// \author Felix Hommel
/// \date 5/24/2026
struct SpaceshipConfiguration
{
    sfa::SpaceshipType type{ sfa::SpaceshipType::XWing };
    sfa::AsyncValue<sfa::SpaceshipColor> color{ sfa::SpaceshipColor::Red };
};

} // namespace

namespace sfa
{

/// \brief The \ref GameSession is managing data that is relevant across multiple screens
///
/// \author Felix Hommel
/// \date 5/24/2026
struct GameSession
{
    ::SpaceshipConfiguration spaceshipConfig;
};

}; // namespace sfa

#endif // !SFA_SRC_ENGINE_SCREENS_GAME_SESSION_HPP
