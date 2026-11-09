#ifndef SFA_SRC_TEST_TEST_UTILITY_OPENGL_TEST_CONTEXT_HPP
#define SFA_SRC_TEST_TEST_UTILITY_OPENGL_TEST_CONTEXT_HPP

#include <glad/gl.h>

#include <GLFW/glfw3.h>

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
    ~OpenGLTestContext();

    OpenGLTestContext(const OpenGLTestContext&) = delete;
    OpenGLTestContext(OpenGLTestContext&&) = delete;
    OpenGLTestContext& operator=(const OpenGLTestContext&) = delete;
    OpenGLTestContext& operator=(OpenGLTestContext&&) = delete;

    /// \brief Initialize the OpenGL context.
    ///
    /// \returns *true* if the context was created successfully, *false* otherwise
    bool setup();
    /// Destroy the OpenGL context.
    void teardown();

    [[nodiscard]] std::string_view getSkipReason() const noexcept { return m_skippingReason; }

private:
    GLFWwindow* m_window{ nullptr };

    bool m_initialized{ false };
    std::string m_skippingReason;

    bool initGLFW();
    bool createWindow();
    bool loadGLAD();
};

} // namespace sfa::testing

#endif // !SFA_SRC_TEST_TEST_UTILITY_OPENGL_TEST_CONTEXT_HPP

