#include "Application.hpp"

#include "Game.hpp"
#include "utility/ResourceManager.hpp"

#include "glad/gl.h"
#include <GLFW/glfw3.h>

#include <memory>
#include <stdexcept>

namespace sfa
{

Application::Application(int width, int height)
    : m_windowInfo{
        .width = width,
        .height = height,
        .refreshRate = REFRESH_RATE,
        .title = APPLICATION_NAME }
{
    initGLFW();
    initGLState();
    registerCallbacks();
    initApplication();
}

Application::~Application()
{
    ResourceManager::clear();
    glfwTerminate();
}

void Application::run()
{
    gameLoop();
}

void Application::framebufferResizeCallbackHandler(int width, int height)
{
    m_windowInfo.width = width;
    m_windowInfo.height = height;
    glViewport(0, 0, m_windowInfo.width, m_windowInfo.height);
}

void Application::cursorPosCallbackHandler(double posX, double posY)
{
    m_mouseInfo.posX = static_cast<float>(posX);
    m_mouseInfo.posY = static_cast<float>(posY);
}

void Application::mouseButtonCallbackHandler(bool isPressed)
{
    m_mouseInfo.isPressed = isPressed;
}

/// \brief Initialize GLFW
///
/// \throws runtime_error if GLFW could not be initialized or window creation failed
void Application::initGLFW()
{
    if(glfwInit() == 0)
        // TODO: Replace with custom exception
        throw std::runtime_error("[ERROR] An error occured while initializing GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    const GLFWvidmode* videomode{ glfwGetVideoMode(glfwGetPrimaryMonitor()) };
    m_windowInfo.width = videomode->width;
    m_windowInfo.height = videomode->height;
    m_windowInfo.refreshRate = videomode->refreshRate;

    m_window = ::createWindow(m_windowInfo.width, m_windowInfo.height, m_windowInfo.title);
    glfwMakeContextCurrent(m_window.get());
    m_windowInfo.window = m_window;

    glfwSetWindowUserPointer(m_window.get(), this);
}

/// \brief Initialize GLAD and OpenGL state machine
///
/// \brief Initialize OpenGL
///
/// \throws runtime_error, if GLAD could not load the OpenGL functions
void Application::initGLState() const
{
    if(gladLoadGL(glfwGetProcAddress) == 0)
        // TODO: Replace with custom exception
        throw std::runtime_error("[ERROR] An error occured while initializing GLAD");

    glViewport(0, 0, static_cast<int>(m_windowInfo.width), static_cast<int>(m_windowInfo.height));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/// \brief Register Callback functions
void Application::registerCallbacks()
{
    glfwSetFramebufferSizeCallback(m_window.get(), ::framebufferResizeCallback);
    glfwSetCursorPosCallback(m_window.get(), ::cursorPosCallback);
    glfwSetMouseButtonCallback(m_window.get(), ::mouseButtonCallback);
}

/// \brief Initialize the Game object
void Application::initApplication()
{
    m_game = std::make_unique<Game>(&m_windowInfo, &m_mouseInfo);
}

/// \brief The main game loop of the Application
///
/// In this function the main loop is executed. The delta time is updated the user inputs are
/// being processed, the masterUpdate() function is called, the screen is cleared and then
/// the new scene is rendered with the masterRender() function.
void Application::gameLoop()
{
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while(glfwWindowShouldClose(m_window.get()) == 0)
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        processInput();
        masterUpdate(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        masterRender();

        glfwSwapBuffers(m_window.get());
    }
}

/// \brief Within the masterUpdate() function, all other update functions are called
///
/// \param dt delta time
void Application::masterUpdate(float dt)
{
    m_game->update(dt);

    if(m_game->getQuit())
        glfwSetWindowShouldClose(m_window.get(), 1);
}

/// \brief Handle user inputs
///
/// \param dt delta time
void Application::processInput()
{
    if(glfwGetKey(m_window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window.get(), 1);
}

/// \brief Within the masterRender() function all other render functions are called
void Application::masterRender()
{
    m_game->render();
}

} // namespace sfa

