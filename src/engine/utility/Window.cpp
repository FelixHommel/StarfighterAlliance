#include "Window.hpp"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace sfa
{

Window::Window(const std::string& title, int width, int height)
    : m_window{ std::unique_ptr<GLFWwindow, decltype(WINDOW_DELETER)>(nullptr, WINDOW_DELETER) }
    , m_width{ width }
    , m_height{ height }
{
    if(glfwInit() == GLFW_FALSE)
        // TODO: replace with custom exception
        throw std::runtime_error("Error occured during GLFW initializtion");

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
        // TODO: replace with custom exception
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window.get());
    glfwSetWindowUserPointer(m_window.get(), this);

    if(gladLoadGL(glfwGetProcAddress) == 0)
    {
        glfwTerminate();
        // TODO: replace with custom exception
        throw std::runtime_error("Error occured during GLAD initializtion");
    }

    glfwSetFramebufferSizeCallback(m_window.get(), [](GLFWwindow* window, int width, int height) {
        auto* self{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
        self->onResize(width, height);
    });
}

Window::~Window()
{
    glfwTerminate();
}

void Window::onResize(int width, int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}

void Window::attachInputController(std::shared_ptr<IInputController> controller)
{
    if(controller == nullptr)
        return;

    m_inputController = std::move(controller);

    glfwSetKeyCallback(m_window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* self{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
        self->m_inputController->onKey(key, scancode, action, mods);
    });
    glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double posX, double posY) {
        auto* self{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
        self->m_inputController->onMoseMove(posX, posY);
    });
    glfwSetMouseButtonCallback(m_window.get(), [](GLFWwindow* window, int button, int action, int mods) {
        auto* self{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
        self->m_inputController->onMouseButton(button, action, mods);
    });
}

} // namespace sfa

