#ifndef SFA_SRC_ENGINE_UTILITY_I_INPUT_CONTROLLER_HPP
#define SFA_SRC_ENGINE_UTILITY_I_INPUT_CONTROLLER_HPP

namespace sfa
{

/// \brief Interface for classes that implement InputController
///
/// \author Felix Hommel
/// \date 2/19/2026
class IInputController
{
public:
    IInputController() = default;
    virtual ~IInputController() = default;

    IInputController(const IInputController&) = default;
    IInputController& operator=(const IInputController&) = default;
    IInputController(IInputController&&) noexcept = delete;
    IInputController& operator=(IInputController&&) noexcept = delete;

    virtual void onKey(int key, int scancode, int action, int mods) = 0;
    virtual void onMouseButton(int button, int action, int mods) = 0;
    virtual void onMoseMove(double posX, double posY) = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_I_INPUT_CONTROLLER_HPP

