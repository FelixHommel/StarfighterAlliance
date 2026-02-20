#ifndef SFA_SRC_ENGINE_UTILITY_I_WINDOW_HPP
#define SFA_SRC_ENGINE_UTILITY_I_WINDOW_HPP

#include "utility/userInput/IInputController.hpp"

#include <memory>

namespace sfa
{

class IWindow
{
public:
    IWindow() = default;
    virtual ~IWindow() = default;

    IWindow(const IWindow&) = delete;
    IWindow& operator=(const IWindow&) = delete;
    IWindow(IWindow&&) noexcept = delete;
    IWindow& operator=(IWindow&&) noexcept = delete;

    virtual void attachInputController(std::shared_ptr<IInputController> controller) = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_I_WINDOW_HPP

