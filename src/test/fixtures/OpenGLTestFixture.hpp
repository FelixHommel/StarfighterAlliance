#ifndef SFA_SRC_TEST_FIXTURES_OPENGL_TEST_FIXTURE_HPP
#define SFA_SRC_TEST_FIXTURES_OPENGL_TEST_FIXTURE_HPP

#include <glad/gl.h>
#include <gtest/gtest.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

namespace sfa
{

/// \brief Test fixture that sets up a simple Headless OpenGL 3.3 context.
///
/// Initialize and set up GLFW to create a simple OpenGL Context and then load OpenGL with GLAD.
///
/// \author Felix Hommel
/// \date 1/24/2026
class OpenGLTestFixture
{
public:
    OpenGLTestFixture() = default;
    ~OpenGLTestFixture() = default;

    OpenGLTestFixture(const OpenGLTestFixture&) = delete;
    OpenGLTestFixture(OpenGLTestFixture&&) = delete;
    OpenGLTestFixture& operator=(const OpenGLTestFixture&) = delete;
    OpenGLTestFixture& operator=(OpenGLTestFixture&&) = delete;

    void setup()
    {
        if(glfwInit() != GLFW_TRUE)
            GTEST_SKIP() << "GLFW initialization failed, skipping test";

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
            GTEST_SKIP() << "GLFW couldn't create a window, skipping test";
        }
        glfwMakeContextCurrent(m_window);

        if(gladLoadGL(glfwGetProcAddress) == 0)
            throw std::runtime_error("Failed to load OpenGL");

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
                    std::cerr << "OpenGL error(" << type << "): " << message << '\n';
            },
            nullptr
        );
    }

    void teardown()
    {
        if(m_window != nullptr)
            glfwDestroyWindow(m_window);

        glfwTerminate();
    }

private:
    static constexpr auto GL_CONTEXT_MAJ{ 3 };
    static constexpr auto GL_CONTEXT_MIN{ 3 };

    GLFWwindow* m_window{ nullptr };
};

} // namespace sfa

#endif // !SFA_SRC_TEST_FIXTURES_OPENGL_TEST_FIXTURE_HPP

