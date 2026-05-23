#include "GLFWWindow.hpp"

#include "utility/exceptions/WindowCreationException.hpp"
#include "utility/userInput/InputController.hpp"
#include "utility/userInput/InputEvent.hpp"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <string>

namespace
{

using namespace sfa;

constexpr Key glfwToKey(int key)
{
    switch(key)
    {
    case GLFW_KEY_W:
        return Key::W;
    case GLFW_KEY_A:
        return Key::A;
    case GLFW_KEY_S:
        return Key::S;
    case GLFW_KEY_D:
        return Key::D;
    case GLFW_KEY_ESCAPE:
        return Key::Esc;
    default:
        return Key::Unknown;
    }
}
constexpr MouseButton glfwToButton(int button)
{
    switch(button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        return MouseButton::Left;
    case GLFW_MOUSE_BUTTON_RIGHT:
        return MouseButton::Right;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        return MouseButton::Middle;
    default:
        return MouseButton::Unknown;
    }
}
constexpr InputAction glfwToAction(int action)
{
    switch(action)
    {
    case GLFW_PRESS:
        return InputAction::Press;
    case GLFW_RELEASE:
        return InputAction::Release;
    default:
        return InputAction::Unknown;
    }
}

constexpr KeyboardInputEvent translateKeyEvent(int key, int action)
{
    return { .key = ::glfwToKey(key), .action = ::glfwToAction(action) };
}

constexpr MouseInputEvent translateMouseEvent(int button, int action)
{
    return { .button = ::glfwToButton(button), .action = ::glfwToAction(action) };
}

constexpr MouseMoveEvent translateMouseMoveEvent(double posX, double posY)
{
    return { .posX = posX, .posY = posY };
}

} // namespace

namespace sfa
{

GLFWWindow::GLFWWindow(const std::string& title, int width, int height)
    : m_window{ std::unique_ptr<GLFWwindow, WindowDeleter>(nullptr, WindowDeleter{}) }
    , m_width{ width }
    , m_height{ height }
{
    if(glfwInit() == GLFW_FALSE)
        throw WindowCreationException("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_VERSION_MAJ);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_VERSION_MIN);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if(m_width <= 0 || m_height <= 0)
    {
        const auto* vm{ glfwGetVideoMode(glfwGetPrimaryMonitor()) };

        m_width = vm->width;
        m_height = vm->height;
    }

    m_window.reset(glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr));

    if(m_window == nullptr)
    {
        glfwTerminate();
        throw WindowCreationException("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window.get());
    glfwSetWindowUserPointer(m_window.get(), this);

    if(gladLoadGL(glfwGetProcAddress) == 0)
    {
        glfwTerminate();
        throw WindowCreationException("Error occured during GLAD initializtion");
    }

    glfwSetFramebufferSizeCallback(m_window.get(), [](GLFWwindow* window, int width, int height) {
        auto* self{ static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window)) };
        self->onResize(width, height);
    });

    spdlog::info("GLFWWindow created successfully");
}

GLFWWindow::~GLFWWindow()
{
    glfwTerminate();
}

void GLFWWindow::onResize(int width, int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}

void GLFWWindow::attachInputController(InputController* controller)
{
    if(controller == nullptr)
        return;

    m_inputController = controller;

    glfwSetKeyCallback(m_window.get(), [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
        auto* self{ static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window)) };
        self->m_inputController->registerEvent(::translateKeyEvent(key, action));
    });
    glfwSetMouseButtonCallback(m_window.get(), [](GLFWwindow* window, int button, int action, int /*mods*/) {
        auto* self{ static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window)) };
        self->m_inputController->registerEvent(::translateMouseEvent(button, action));
    });
    glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double posX, double posY) {
        auto* self{ static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window)) };
        self->m_inputController->registerEvent(::translateMouseMoveEvent(posX, posY));
    });
}

} // namespace sfa

