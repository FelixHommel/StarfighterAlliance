#ifndef SFA_SRC_ENGINE_UTILITY_INPUT_CONTROLLER_HPP
#define SFA_SRC_ENGINE_UTILITY_INPUT_CONTROLLER_HPP

#include "utility/IInputController.hpp"
#include <array>
#include <cstddef>

namespace sfa
{

class InputController : public IInputController
{
public:
    InputController() = default;
    ~InputController() override = default;

    InputController(const InputController&) = default;
    InputController& operator=(const InputController&) = default;
    InputController(InputController&&) noexcept = delete;
    InputController& operator=(InputController&&) noexcept = delete;

    void onKey(int key, int scancode, int action, int mods) override;
    void onMouseButton(int button, int action, int mods) override;
    void onMoseMove(double posX, double posY) override;

private:
    static constexpr std::size_t ARRAY_SIZE{ 50 };

    std::array<bool, ARRAY_SIZE> m_keyStates{};
    std::array<bool, ARRAY_SIZE> m_mouseStates{};
    double m_mousePosX{ 0 };
    double m_mousePosY{ 0 };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_INPUT_CONTROLLER_HPP

