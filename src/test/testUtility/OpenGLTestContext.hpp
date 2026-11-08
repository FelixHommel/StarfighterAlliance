#ifndef SFA_SRC_TEST_TEST_UTILITY_OPENGL_TEST_CONTEXT_HPP
#define SFA_SRC_TEST_TEST_UTILITY_OPENGL_TEST_CONTEXT_HPP

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <print>
#include <string>
#include <string_view>

namespace sfa::testing
{

/// \brief Test fixture that sets up a simple Headless OpenGL 3.3 context.
///
/// Initialize and set up GLFW to create a simple OpenGL Context and then load OpenGL with GLAD.
///
/// \author Felix Hommel
/// \date 1/24/2026
class OpenGLTestContext
{
public:
    OpenGLTestContext() = default;
    ~OpenGLTestContext() = default;

    OpenGLTestContext(const OpenGLTestContext&) = delete;
    OpenGLTestContext(OpenGLTestContext&&) = delete;
    OpenGLTestContext& operator=(const OpenGLTestContext&) = delete;
    OpenGLTestContext& operator=(OpenGLTestContext&&) = delete;

    bool setup()
    {
        if(glfwInit() != GLFW_TRUE)
        {
            m_skippingReason = "GLFW initialization failed, skipping test";

            return false;
        }

        // NOTE: Configure GLFW to create a Headless OpenGL 3.3 Context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_CONTEXT_MAJ);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_CONTEXT_MIN);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        m_window = glfwCreateWindow(1, 1, "Test", nullptr, nullptr);
        if(m_window == nullptr)
        {
            glfwTerminate();

            m_initialized = false;
            m_skippingReason = "GLFW couldn't create a window, skipping test";

            return false;
        }
        glfwMakeContextCurrent(m_window);

        if(gladLoadGL(glfwGetProcAddress) == 0)
        {
            glfwDestroyWindow(m_window);
            glfwTerminate();

            m_window = nullptr;
            m_initialized = false;
            m_skippingReason = "Failed to load OpenGL, skipping test";

            return false;
        }

        m_skippingReason.clear();

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

        return true;
    }

    void teardown()
    {
        if(!m_initialized)
            return;

        if(m_window != nullptr)
            glfwDestroyWindow(m_window);

        glfwTerminate();

        m_window = nullptr;
        m_initialized = false;
    }

    [[nodiscard]] std::string_view getSkipReason() const noexcept { return m_skippingReason; }

private:
    static constexpr auto GL_CONTEXT_MAJ{ 3 };
    static constexpr auto GL_CONTEXT_MIN{ 3 };

    GLFWwindow* m_window{ nullptr };

    bool m_initialized{ false };
    std::string m_skippingReason;
};

} // namespace sfa

#endif // !SFA_SRC_TEST_TEST_UTILITY_OPENGL_TEST_CONTEXT_HPP

