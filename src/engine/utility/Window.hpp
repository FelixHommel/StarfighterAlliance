#ifndef SFA_SRC_ENGINE_UTILITY_WINDOW_HPP
#define SFA_SRC_ENGINE_UTILITY_WINDOW_HPP

#include "utility/IInputController.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

namespace sfa
{

struct Viewport
{
    int width;
    int height;
};

/// \brief Abstraction for GLFW window and context management.
///
/// \author Felix Hommel
/// \date 2/19/2026
class Window
{
public:
    /// \brief Create a windows with explicitly specified with and height.
    ///
    /// \note If either width or height are <= 0, the \ref Window will be created in full screen mode
    ///
    /// \param title the title of the window
    /// \param width (optional) the with of the window
    /// \param height (optional) the height of the window
    Window(const std::string& title, int width = 0, int height = 0);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    [[nodiscard]] Viewport viewport() const noexcept { return { .width = m_width, .height = m_height }; }

    /// \brief Callback handler for window resize events
    ///
    /// \param width the new window width
    /// \param height the new window height
    void onResize(int width, int height);

    /// \brief Attach input controller and register the input controller callbacks with GLFW
    ///
    /// \param controller 
    void attachInputController(std::shared_ptr<IInputController> controller);

private:
    static constexpr auto WINDOW_DELETER{ [](GLFWwindow* window) { glfwDestroyWindow(window); } }; ///< GLFWwindow deleter lambda
    static constexpr int CONTEXT_VERSION_MAJ{ 3 };
    static constexpr int CONTEXT_VERSION_MIN{ 3 };

    std::unique_ptr<GLFWwindow, decltype(WINDOW_DELETER)> m_window;
    int m_width;
    int m_height;

    std::shared_ptr<IInputController> m_inputController{ nullptr };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_WINDOW_HPP

