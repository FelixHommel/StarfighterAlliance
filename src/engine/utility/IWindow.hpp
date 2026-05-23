#ifndef SFA_SRC_ENGINE_UTILITY_I_WINDOW_HPP
#define SFA_SRC_ENGINE_UTILITY_I_WINDOW_HPP

#include "utility/userInput/InputController.hpp"

namespace sfa
{

/// \brief Simple struct containing the dimensions of the window.
///
/// \author Felix Hommel
/// \date 2/19/2026
struct Viewport
{
    int width;
    int height;
};


class IWindow
{
public:
    IWindow() = default;
    virtual ~IWindow() = default;

    IWindow(const IWindow&) = delete;
    IWindow& operator=(const IWindow&) = delete;
    IWindow(IWindow&&) noexcept = delete;
    IWindow& operator=(IWindow&&) noexcept = delete;

    [[nodiscard]] virtual bool shouldClose() const = 0;
    [[nodiscard]] virtual Viewport viewport() const noexcept = 0;

    virtual void setShouldClose() = 0;

    virtual void attachInputController(InputController* controller) = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_I_WINDOW_HPP

