#ifndef SFA_SRC_ENGNIE_APPLICATION_HPP
#define SFA_SRC_ENGNIE_APPLICATION_HPP

#include "Game.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <memory>
#include <stdexcept>

namespace sfa
{

/// \brief The Application class is the general orchestrator that sets up everything around the game
///
/// The Application class handles all the GLFW window setup process and sets up all the callbacks
///
/// \author Felix Hommel
/// \date Nov 16, 2024
class Application
{
public:
    /// \brief Create a new Application object
    ///
    /// \param width the width of the window
    /// \param height the height of the window
    Application(int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT);
    ~Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    /// \brief Start the game by initializing everything and then starting the main game loop
    ///
    /// The order of initialization is important.
    ///   1. GLFW – Windowing
    ///   3. GLState – OpenGL Function loading and OpenGL settings
    ///   4. Callbacks – GLFW Callbacks
    ///   5. Application – Prepare for running the game
    void run();

    /// \brief Callback handler for framebuffer resizes
    ///
    /// \param width the new width of the framebuffer
    /// \param height the new height of the framebuffer
    void framebufferResizeCallbackHandler(int width, int height);
    /// \brief Callback handler for mouse movement
    ///
    /// \param posX current x-axis position of the cursor on the window
    /// \param posY current y-axis position of the cursor on the window
    void cursorPosCallbackHandler(double posX, double posY);
    /// \brief Callback handler for mouse button presses
    ///
    /// \param isPressed whether or not a mouse button is pressed
    void mouseButtonCallbackHandler(bool isPressed);

private:
    static constexpr auto DEFAULT_WINDOW_WIDTH{ 960 };
    static constexpr auto DEFAULT_WINDOW_HEIGHT{ 540 };
    static constexpr auto REFRESH_RATE{ 60 };
    static constexpr auto APPLICATION_NAME{ "Starfighter Alliance" };

    std::shared_ptr<GLFWwindow> m_window;
    std::unique_ptr<Game> m_game;
    WindowInfo m_windowInfo;
    Mouse m_mouseInfo{};

    void initGLFW();
    void initGLAD();
    void initGLState() const;
    void registerCallbacks();
    void initApplication();

    void gameLoop();

    void processInput();
    void masterUpdate(float dt);
    void masterRender();
};

} // namespace sfa

namespace
{

void framebufferResizeCallback(GLFWwindow* pWindow, int width, int height)
{
    auto* app{ reinterpret_cast<sfa::Application*>(glfwGetWindowUserPointer(pWindow)) };
    app->framebufferResizeCallbackHandler(width, height);
}

void cursorPosCallback(GLFWwindow* pWindow, double posX, double posY)
{
    auto* app{ reinterpret_cast<sfa::Application*>(glfwGetWindowUserPointer(pWindow)) };
    app->cursorPosCallbackHandler(posX, posY);
}

void mouseButtonCallback(GLFWwindow* pWindow, int /*button*/, int action, int /*mods*/)
{
    auto* app{ reinterpret_cast<sfa::Application*>(glfwGetWindowUserPointer(pWindow)) };
    app->mouseButtonCallbackHandler(action == GLFW_PRESS);
}

std::shared_ptr<GLFWwindow> createWindow(int width, int height, const std::string& title)
{
    GLFWwindow* window{ glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr) };

    if(window == nullptr)
        // TODO: Replace with custom exception
        throw std::runtime_error("[ERROR] Failure while creating a window");

    return { window, [](GLFWwindow* pWindow) { glfwDestroyWindow(pWindow); } };
}

} // namespace

#endif //! SFA_SRC_ENGNIE_APPLICATION_HPP

