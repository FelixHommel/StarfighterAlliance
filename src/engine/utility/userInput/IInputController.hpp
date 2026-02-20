#ifndef SFA_SRC_ENGINE_UTILITY_I_INPUT_CONTROLLER_HPP
#define SFA_SRC_ENGINE_UTILITY_I_INPUT_CONTROLLER_HPP

#include "utility/userInput/InputEvent.hpp"

namespace sfa
{

/// \brief Interface for classes that implement are capable of processing user input.
///
/// \author Felix Hommel
/// \date 2/19/2026
class IInputController
{
public:
    IInputController() = default;
    virtual ~IInputController() = default;

    IInputController(const IInputController&) = delete;
    IInputController& operator=(const IInputController&) = delete;
    IInputController(IInputController&&) noexcept = delete;
    IInputController& operator=(IInputController&&) noexcept = delete;

    /// \brief Register an incoming event.
    ///
    /// \param event the event that is incmoing
    virtual void registerEvent(const InputEvent& event) = 0;
    /// \brief Check if a certain key is currently pressed
    ///
    /// \param key the Key to be checked
    ///
    /// \returns *true* if \p key is pressed, *false* if not
    [[nodiscard]] virtual bool isKeyPressed(Key key) const = 0;
    /// \brief Check if a certain mouse button is currently pressed.
    ///
    /// \param button the button to be checked
    ///
    /// \returns *true* if \p button is pressed, *false* if not
    [[nodiscard]] virtual bool isMousePressed(MouseButton button) const = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_I_INPUT_CONTROLLER_HPP

