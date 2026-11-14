#ifndef SFA_SRC_ENGINE_SCREENS_SCREEN_COMMAND_HPP
#define SFA_SRC_ENGINE_SCREENS_SCREEN_COMMAND_HPP

#include "screens/IScreen.hpp"

#include <cstdint>
#include <memory>

namespace sfa
{

/// \brief A \ref ScreenCommand can be used to signal the \ref ScreenStack to perform a screen transition.
///
/// \author Felix Hommel
/// \date 3/4/2026
struct ScreenCommand
{
    /// \brief Indicate what kind of transition this command indicates.
    ///
    /// \author Felix Hommel
    /// \date 3/4/2026
    enum class Type : std::uint8_t
    {
        Push,
        Pop,
        Replace
    };

    Type type;
    std::unique_ptr<IScreen> screen;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_SCREENS_SCREEN_COMMAND_HPP

