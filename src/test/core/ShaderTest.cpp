#include "core/Shader.hpp"
#include "fixtures/OpenGLTestFixture.hpp"
#include "testUtility/RandomNumberGenerator.hpp"

#include <gtest/gtest.h>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <cstddef>
#include <memory>
#include <span>
#include <type_traits>
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
        if(!m_glContext->setup())
            GTEST_SKIP() << m_glContext->getSkipReason();

        m_shader = std::make_unique<Shader>(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);

        m_intLocation = glGetUniformLocation(m_shader->getID(), INT_UNIFORM);
        m_floatLocation = glGetUniformLocation(m_shader->getID(), FLOAT_UNIFORM);
        m_float2Location = glGetUniformLocation(m_shader->getID(), FLOAT_2_UNIFORM);
        m_float3Location = glGetUniformLocation(m_shader->getID(), FLOAT_3_UNIFORM);
        m_float4Location = glGetUniformLocation(m_shader->getID(), FLOAT_4_UNIFORM);
        m_matrix4Location = glGetUniformLocation(m_shader->getID(), MATRIX_4_UNIFORM);
    }

    void TearDown() override { m_glContext->teardown(); }

protected:
    static constexpr auto VERTEX_SHADER_SRC{ R"(
        #version 330 core

        uniform int intUniform;
        uniform float floatUniform;
        uniform vec2 float2Uniform;
        uniform vec3 float3Uniform;
        uniform vec4 float4Uniform;
        uniform mat4 mat4Uniform;

        void main()
        {
            vec4 result = vec4(float4Uniform.xyz + float3Uniform, float4Uniform.w);
            result.xy += float2Uniform;
            result.x += intUniform;
            result *= floatUniform;
            result = mat4Uniform * result;
            gl_Position = normalize(result);
        }
    )" };

    static constexpr auto FRAGMENT_SHADER_SRC{ R"(
        #version 330 core
        out vec4 FragColor;

        void main() { FragColor = vec4(1.0); }
    )" };

    static constexpr auto GEOMETRY_SHADER_SRC{ R"(
        #version 330 core
        layout(points) in;
        layout(line_strip, max_vertices = 2) out;

        void main() {
            gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
            EmitVertex();
            gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
            EmitVertex();
            EndPrimitive();
        }
    )" };
    static constexpr auto INT_UNIFORM{ "intUniform" };
    static constexpr auto FLOAT_UNIFORM{ "floatUniform" };
    static constexpr auto FLOAT_2_UNIFORM{ "float2Uniform" };
    static constexpr auto FLOAT_3_UNIFORM{ "float3Uniform" };
    static constexpr auto FLOAT_4_UNIFORM{ "float4Uniform" };
    static constexpr auto MATRIX_4_UNIFORM{ "mat4Uniform" };

    std::unique_ptr<OpenGLTestFixture> m_glContext{ std::make_unique<OpenGLTestFixture>() };
    std::unique_ptr<Shader> m_shader;

    int m_intLocation{ 0 };
    int m_floatLocation{ 0 };
    int m_float2Location{ 0 };
    int m_float3Location{ 0 };
    int m_float4Location{ 0 };
    int m_matrix4Location{ 0 };

    template<typename T>
        requires std::is_floating_point_v<T> || std::is_integral_v<T>
    void getUniformValue(int location, T& out)
    {
        if constexpr(std::is_same_v<T, GLfloat>)
            glGetUniformfv(m_shader->getID(), location, &out);
        else if constexpr(std::is_same_v<T, GLint>)
            glGetUniformiv(m_shader->getID(), location, &out);
        else if constexpr(std::is_same_v<T, GLuint>)
            glGetUniformuiv(m_shader->getID(), location, &out);
        else
            static_assert(!sizeof(T), "Unsupported uniform element type for glGetUniform");
    }

    template<typename T>
    void getUniformValue(int location, std::span<T> out)
    {
        if constexpr(std::is_same_v<T, GLfloat>)
            glGetUniformfv(m_shader->getID(), location, out.data());
        else if constexpr(std::is_same_v<T, GLint>)
            glGetUniformiv(m_shader->getID(), location, out.data());
        else if constexpr(std::is_same_v<T, GLuint>)
            glGetUniformuiv(m_shader->getID(), location, out.data());
        else
            static_assert(!sizeof(T), "Unsupported uniform element type for glGetUniform");
    }
};

/// \brief Test the \ref Shader RAII behavior.
///
/// The constructor should create a valid OpenGL shader program and the destructor should delete the program.
TEST_F(ShaderTest, ShaderRAII)
{
    unsigned int originalID{};

    {
        const Shader original(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC, GEOMETRY_SHADER_SRC);
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

/// \brief Test the \ref Shader move assignment when assigning to the same object.
///
/// When the move target is the same as the move source, nothing should change.
TEST_F(ShaderTest, ShaderMoveAssignmentOnSameShader)
{
    Shader shader(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
    const unsigned int originalShaderID{ shader.getID() };

    shader = std::move(shader);

    EXPECT_TRUE(static_cast<bool>(glIsProgram(originalShaderID)));
    EXPECT_EQ(originalShaderID, shader.getID());
}

/// \brief Test the \ref Shader use method.
///
/// When the shader is being activated with the use method, OpenGL should report it back as the currently used program.
TEST_F(ShaderTest, ShaderUseActivatesProgram)
{
    m_shader->use();

    int currentProgram{ 0 };
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

    ASSERT_NE(0, currentProgram);
    EXPECT_EQ(currentProgram, m_shader->getID());
}

/// \brief Test uploading a single float uniform value
///
/// When a value is uploaded to a uniform, the shader should receive the uploaded value
TEST_F(ShaderTest, SetSingleFloatingValueWithUse)
{
    const auto val{ generateRandomValue<float>() };
    m_shader->setFloat(FLOAT_UNIFORM, val, true);

    GLfloat uniformValue{ 0 };
    getUniformValue<GLfloat>(m_floatLocation, uniformValue);

    EXPECT_FLOAT_EQ(val, uniformValue);
}

/// \brief Test uploading a single integer uniform value
///
/// When a value is uploaded to a uniform, the shader should receive the uploaded value
TEST_F(ShaderTest, SetSingleIntegerValueWithUse)
{
    const auto val{ generateRandomValue<int>() };
    m_shader->setInteger(INT_UNIFORM, val, true);

    GLint uniformValue{ 0 };
    getUniformValue<GLint>(m_intLocation, uniformValue);

    EXPECT_EQ(val, uniformValue);
}

/// \brief Test uploading a 2 element float vector uniform value using 2 individual values
///
/// When a value is uploaded to a uniform, the shader should receive the uploaded value
TEST_F(ShaderTest, SetTwoSingleFloatValuesWithUse)
{
    const std::array original{ generateRandomValue<float>(), generateRandomValue<float>() };

    m_shader->setVector2f(FLOAT_2_UNIFORM, original[0], original[1], true);

    std::array<GLfloat, original.size()> uniformValues{};
    getUniformValue<GLfloat>(m_float2Location, uniformValues);

    EXPECT_EQ(original, uniformValues);
}

/// \brief Test uploading a 2 element float vector uniform value using a glm::vec2
///
/// When a value is uploaded to a uniform, the shader should receive the uploaded value
TEST_F(ShaderTest, SetFloatVector2ValueWithUse)
{
    const auto vec{ glm::vec2(generateRandomValue<float>(), generateRandomValue<float>()) };

    m_shader->setVector2f(FLOAT_2_UNIFORM, vec, true);

    std::array<GLfloat, glm::vec2::length()> uniformValues{};
    getUniformValue<GLfloat>(m_float2Location, uniformValues);

    EXPECT_FLOAT_EQ(vec.x, uniformValues.at(0));
    EXPECT_FLOAT_EQ(vec.y, uniformValues.at(1));
}

/// \brief Test uploading a 3 element float vector uniform value using 3 individual values
///
/// When a value is uploaded to a uniform, the shader should receive the uploaded value
TEST_F(ShaderTest, SetThreeSingleFloatValuesWithUse)
{
    const std::array original{ generateRandomValue<float>(),
                               generateRandomValue<float>(),
                               generateRandomValue<float>() };

    m_shader->setVector3f(FLOAT_3_UNIFORM, original[0], original[1], original[2], true);

    std::array<GLfloat, original.size()> uniformValues{};
    glGetUniformfv(m_shader->getID(), m_float3Location, uniformValues.data());

    EXPECT_EQ(original, uniformValues);
}

/// \brief Test uploading a 3 element float vectir uniform value using a glm::vec3
///
/// When a value is uploaded to a uniform, the shader should receive the uploaded value
TEST_F(ShaderTest, SetFloatVector3ValueWithUse)
{
    const auto vec{
        glm::vec3(generateRandomValue<float>(), generateRandomValue<float>(), generateRandomValue<float>())
    };

    m_shader->setVector3f(FLOAT_3_UNIFORM, vec, true);

    std::array<GLfloat, glm::vec3::length()> uniformValues{};
    glGetUniformfv(m_shader->getID(), m_float3Location, uniformValues.data());

    EXPECT_FLOAT_EQ(vec.x, uniformValues.at(0));
    EXPECT_FLOAT_EQ(vec.y, uniformValues.at(1));
    EXPECT_FLOAT_EQ(vec.z, uniformValues.at(2));
}

/// \brief Test uploading a 4 element float vector uniform value using 4 individual values
///
/// When a value is uploaded to a uniform, the shader should receive the uploaded value
TEST_F(ShaderTest, SetFourSingleFloatValuesWithUse)
{
    const std::array original{ generateRandomValue<float>(),
                               generateRandomValue<float>(),
                               generateRandomValue<float>(),
                               generateRandomValue<float>() };

    // NOLINTNEXTLINE(readability-magic-numbers): 3 used as array index
    m_shader->setVector4f(FLOAT_4_UNIFORM, original[0], original[1], original[2], original[3], true);

    std::array<GLfloat, original.size()> uniformValues{};
    glGetUniformfv(m_shader->getID(), m_float4Location, uniformValues.data());

    EXPECT_EQ(original, uniformValues);
}

/// \brief Test uploading a 4 element float vector uniform value using a single glm::vec4
///
/// When a value is uploaded to a uniform, the shader should receive the uploaded value
TEST_F(ShaderTest, SetFloatVector4ValueWithUse)
{
    const auto vec{ glm::vec4(
        generateRandomValue<float>(),
        generateRandomValue<float>(),
        generateRandomValue<float>(),
        generateRandomValue<float>()
    ) };

    m_shader->setVector4f(FLOAT_4_UNIFORM, vec, true);

    std::array<GLfloat, glm::vec4::length()> uniformValues{};
    glGetUniformfv(m_shader->getID(), m_float4Location, uniformValues.data());

    EXPECT_FLOAT_EQ(vec.x, uniformValues.at(0));
    EXPECT_FLOAT_EQ(vec.y, uniformValues.at(1));
    EXPECT_FLOAT_EQ(vec.z, uniformValues.at(2));
    EXPECT_FLOAT_EQ(vec.w, uniformValues.at(3));
}

/// \brief Test uploading a 4x4 float matrix uniform value
///
/// When a value is uploaded to a uniform, the shader should receive the uploaded value
TEST_F(ShaderTest, SetMatrix4ValueWithUse)
{
    const auto mat{ glm::mat4(generateRandomValue<float>()) };

    m_shader->setMatrix4(MATRIX_4_UNIFORM, mat, true);

    std::array<GLfloat, static_cast<std::size_t>(glm::mat4::length() * glm::mat4::length())> uniformValues{};
    getUniformValue<GLfloat>(m_matrix4Location, uniformValues);

    const glm::mat4 result{ glm::make_mat4(uniformValues.data()) };
    EXPECT_EQ(mat, result);
}

} // namespace sfa::testing

