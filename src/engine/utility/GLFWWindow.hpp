#ifndef SFA_SRC_ENGINE_UTILITY_WINDOW_HPP
#define SFA_SRC_ENGINE_UTILITY_WINDOW_HPP

#include "utility/IWindow.hpp"
#include "utility/userInput/InputController.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

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

/// \brief Abstraction for GLFW window and context management.
///
/// \author Felix Hommel
/// \date 2/19/2026
class GLFWWindow : public IWindow
{
public:
    /// \brief Create a windows with explicitly specified with and height.
    ///
    /// \note If either width or height are <= 0, the \ref Window will be created in full screen mode
    ///
    /// \param title the title of the window
    /// \param width (optional) the with of the window
    /// \param height (optional) the height of the window
    explicit GLFWWindow(const std::string& title, int width = 0, int height = 0);
    ~GLFWWindow() override;

    GLFWWindow(const GLFWWindow&) = delete;
    GLFWWindow& operator=(const GLFWWindow&) = delete;
    GLFWWindow(GLFWWindow&&) = delete;
    GLFWWindow& operator=(GLFWWindow&&) = delete;

    [[nodiscard]] Viewport viewport() const noexcept { return { .width = m_width, .height = m_height }; }
    [[nodiscard]] bool shouldClose() const override { return glfwWindowShouldClose(m_window.get()) == GLFW_TRUE; }

    /// \brief Attach input controller and register the input controller callbacks with GLFW
    ///
    /// \param controller
    void attachInputController(InputController* controller) override;

    /// \brief Callback handler for window resize events
    ///
    /// \param width the new window width
    /// \param height the new window height
    void onResize(int width, int height);

    /// \brief Signal the window that it should be closing
    void setShouldClose() { glfwSetWindowShouldClose(m_window.get(), GLFW_TRUE); }

private:
    struct WindowDeleter
    {
        void operator()(GLFWwindow* window) const noexcept { glfwDestroyWindow(window); }
    };

    static constexpr int CONTEXT_VERSION_MAJ{ 3 };
    static constexpr int CONTEXT_VERSION_MIN{ 3 };

    std::unique_ptr<GLFWwindow, WindowDeleter> m_window;
    int m_width;
    int m_height;

    InputController* m_inputController{ nullptr };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_WINDOW_HPP

