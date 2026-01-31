#include "core/Shader.hpp"
#include "fixtures/OpenGLTestFixture.hpp"

#include <gtest/gtest.h>
#include <glad/gl.h>

#include <memory>
#include <utility>

namespace sfa::testing
{

/// \brief Test the features of the \ref Shader class.
///
/// \author Felix Hommel
/// \date 1/24/2026
class ShaderTest : public ::testing::Test
{
public:
    ShaderTest() = default;
    ~ShaderTest() override = default;

    ShaderTest(const ShaderTest&) = delete;
    ShaderTest(ShaderTest&&) = delete;
    ShaderTest& operator=(const ShaderTest&) = delete;
    ShaderTest& operator=(ShaderTest&&) = delete;

    void SetUp() override
    {
        m_glContext->setup();
    }

    void TearDown() override
    {
        m_glContext->teardown();
    }

protected:
    static constexpr auto VERTEX_SHADER_SRC = R"(
        #version 330 core

        void main() { gl_Position = vec4(1.0); }
    )";

    static constexpr auto FRAGMENT_SHADER_SRC = R"(
        #version 330 core
        out vec4 FragColor;

        void main() { FragColor = vec4(1.0); }
    )";

    std::unique_ptr<OpenGLTestFixture> m_glContext{ std::make_unique<OpenGLTestFixture>() };
};

/// \brief Test the \ref Shader RAII behavior.
///
/// The constructor should create a valid OpenGL shader program and the destructor should delete the program.
TEST_F(ShaderTest, ShaderRAII)
{
    unsigned int originalID{};
    {
        const Shader original(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
        originalID = original.getID();

        EXPECT_NE(originalID, 0);
        EXPECT_TRUE(static_cast<bool>(glIsProgram(originalID)));
    }

    EXPECT_FALSE(static_cast<bool>(glIsProgram(originalID)));
}

/// \brief Test the \ref Shader move constructor.
///
/// Move constructing a \ref Shader should preserve the underlying shader program ID.
TEST_F(ShaderTest, ShaderMoveConstructor)
{
    Shader original(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
    const unsigned int originalID{ original.getID() };
    Shader moved{ std::move(original) };

    EXPECT_EQ(moved.getID(), originalID);
    EXPECT_TRUE(static_cast<bool>(glIsProgram(moved.getID())));
}

/// \brief Test the \ref Shader move assignment.
///
/// Move assignment should preserve the move source's shader program and clean up the resource held in the move target
/// before assigning the move source to it.
TEST_F(ShaderTest, ShaderMoveAssignment)
{
    Shader shader1(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
    const unsigned int originalShader1ID{ shader1.getID() };
    Shader shader2(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
    const unsigned int originalShader2ID{ shader2.getID() };

    shader2 = std::move(shader1);

    EXPECT_FALSE(static_cast<bool>(glIsProgram(originalShader2ID)));
    EXPECT_EQ(originalShader1ID, shader2.getID());
    EXPECT_TRUE(static_cast<bool>(glIsProgram(shader2.getID())));
}

/// \brief Test the \ref Shader use method.
///
/// When the shader is being activated with the use method, OpenGL should report it back as the currently used program.
TEST_F(ShaderTest, ShaderUseActivatesProgram)
{
    Shader shader(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
    shader.use();

    int currentProgram{ 0 };
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

    ASSERT_NE(0, currentProgram);
    EXPECT_EQ(currentProgram, shader.getID());
}

} // namespace sfa::testing

