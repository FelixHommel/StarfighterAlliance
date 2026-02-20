#ifndef SFA_SRC_ENGINE_UTILITY_WINDOW_HPP
#define SFA_SRC_ENGINE_UTILITY_WINDOW_HPP

#include "utility/IWindow.hpp"
#include "utility/userInput/IInputController.hpp"
#include "utility/userInput/InputEvent.hpp"

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
    GLFWWindow(const std::string& title, int width = 0, int height = 0);
    ~GLFWWindow() override;

    GLFWWindow(const GLFWWindow&) = delete;
    GLFWWindow& operator=(const GLFWWindow&) = delete;
    GLFWWindow(GLFWWindow&&) = delete;
    GLFWWindow& operator=(GLFWWindow&&) = delete;

    [[nodiscard]] Viewport viewport() const noexcept { return { .width = m_width, .height = m_height }; }

    /// \brief Callback handler for window resize events
    ///
    /// \param width the new window width
    /// \param height the new window height
    void onResize(int width, int height);

    /// \brief Attach input controller and register the input controller callbacks with GLFW
    ///
    /// \param controller 
    void attachInputController(std::shared_ptr<IInputController> controller) override;

private:
    static constexpr auto WINDOW_DELETER{ [](GLFWwindow* window) { glfwDestroyWindow(window); } }; ///< GLFWwindow deleter lambda
    static constexpr int CONTEXT_VERSION_MAJ{ 3 };
    static constexpr int CONTEXT_VERSION_MIN{ 3 };

    std::unique_ptr<GLFWwindow, decltype(WINDOW_DELETER)> m_window;
    int m_width;
    int m_height;

    std::shared_ptr<IInputController> m_inputController{ nullptr };

    static constexpr Key glfwToKey(int key)
    {
        switch(key)
        {
        case GLFW_KEY_W: return Key::W;
        case GLFW_KEY_A: return Key::A;
        case GLFW_KEY_S: return Key::S;
        case GLFW_KEY_D: return Key::D;
        case GLFW_KEY_ESCAPE: return Key::Esc;
        default: return Key::Unknown;
        }
    }
    static constexpr MouseButton glfwToButton(int button)
    {
        switch(button)
        {
        case GLFW_MOUSE_BUTTON_LEFT: return MouseButton::Left;
        case GLFW_MOUSE_BUTTON_RIGHT: return MouseButton::Right;
        case GLFW_MOUSE_BUTTON_MIDDLE: return MouseButton::Middle;
        default: return MouseButton::Unknown;
        }
    }
    static constexpr InputAction glfwToAction(int action)
    {
        switch(action)
        {
        case GLFW_PRESS: return InputAction::Press;
        case GLFW_RELEASE: return InputAction::Release;
        default: return InputAction::Unknown;
        }
    }
    static constexpr KeyboardInputEvent translateKeyEvent(int key, int action);
    static constexpr MouseInputEvent translateMouseEvent(int button, int action);
    static constexpr MouseMoveEvent translateMouseMoveEvent(double posX, double posY);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_WINDOW_HPP

