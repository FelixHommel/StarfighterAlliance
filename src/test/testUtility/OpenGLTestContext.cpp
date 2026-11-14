#include "OpenGLTestContext.hpp"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <print>
#include <string>

namespace
{

constexpr auto GL_CONTEXT_MAJ{ 3 };
constexpr auto GL_CONTEXT_MIN{ 3 };

/// \brief Configure the OpenGL state for debugging purposes.
void setupGLState()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(
        [](GLenum /*source*/,
           GLenum type,
           GLuint /*id*/,
           GLenum severity,
           GLsizei /*length*/,
           const GLchar* message,
           const void* /*user*/) {
            if(severity == GL_DEBUG_SEVERITY_HIGH)
                std::println("OpenGL error({}): {}", type, message);
        },
        nullptr
    );
}

} // namespace

namespace sfa::testing
{

OpenGLTestContext::~OpenGLTestContext()
{
    teardown();
}

bool OpenGLTestContext::setup()
{
    if(!initGLFW())
        return false;

    if(!createWindow())
        return false;

    glfwMakeContextCurrent(m_window);

    if(!loadGLAD())
        return false;

    ::setupGLState();

    return true;
}

void OpenGLTestContext::teardown()
{
    if(!m_initialized)
        return;

    if(m_window != nullptr)
        glfwDestroyWindow(m_window);

    glfwTerminate();

    m_window = nullptr;
    m_initialized = false;
}

/// \brief Initialize GLFW.
///
/// \returns *true* if GLFW was initialized successfully, *false* otherwise
bool OpenGLTestContext::initGLFW()
{
    if(glfwInit() == GLFW_TRUE)
        return true;

    m_skippingReason = "GLFW initialization failed, skipping test";
    return false;
}

/// \brief Create a GLFW window.
///
/// \returns *true* if the window was created successfully, *false* otherwise
bool OpenGLTestContext::createWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ::GL_CONTEXT_MAJ);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ::GL_CONTEXT_MIN);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    m_window = glfwCreateWindow(1, 1, "Test", nullptr, nullptr);
    if(m_window != nullptr)
        return true;

    glfwTerminate();

    m_initialized = false;
    m_skippingReason = "GLFW couldn't create a window, skipping test";

    return false;
}

/// \brief Load GLAD.
///
/// \returns *true* if GLAD was loaded successfully, *false* otherwise
bool OpenGLTestContext::loadGLAD()
{
    if(gladLoadGL(glfwGetProcAddress) != 0)
        return true;

    glfwDestroyWindow(m_window);
    glfwTerminate();

    m_window = nullptr;
    m_initialized = false;
    m_skippingReason = "Failed to load OpenGL, skipping test";

    return false;
}

} // namespace sfa::testing

