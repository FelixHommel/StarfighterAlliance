#include "core/Shader.hpp"
#include "fixtures/OpenGLTestFixture.hpp"
#include "testUtility/RandomNumberGenerator.hpp"

#include <catch2/catch_test_macros.hpp>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <cstddef>
#include <span>
#include <type_traits>
#include <utility>

namespace sfa::testing
{

template<typename T>
    requires std::is_floating_point_v<T> || std::is_integral_v<T>
void getUniformValue(const Shader& shader, int location, T& out)
{
    if constexpr(std::is_same_v<T, GLfloat>)
        glGetUniformfv(shader.getID(), location, &out);
    else if constexpr(std::is_same_v<T, GLint>)
        glGetUniformiv(shader.getID(), location, &out);
    else if constexpr(std::is_same_v<T, GLuint>)
        glGetUniformuiv(shader.getID(), location, &out);
    else
        static_assert(!sizeof(T), "Unsupported uniform element type for glGetUniform");
}

template<typename T>
void getUniformValue(const Shader& shader, int location, std::span<T> out)
{
    if constexpr(std::is_same_v<T, GLfloat>)
        glGetUniformfv(shader.getID(), location, out.data());
    else if constexpr(std::is_same_v<T, GLint>)
        glGetUniformiv(shader.getID(), location, out.data());
    else if constexpr(std::is_same_v<T, GLuint>)
        glGetUniformuiv(shader.getID(), location, out.data());
    else
        static_assert(!sizeof(T), "Unsupported uniform element type for glGetUniform");
}

/// \brief Test the features of the \ref Shader class.
///
/// \author Felix Hommel
/// \date 1/24/2026
TEST_CASE("ShaderTest", "[unit][core][opengl]")
{
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

    OpenGLTestFixture glContext{};

    if(!glContext.setup())
        SKIP(glContext.getSkipReason());

    Shader shader{ VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC };

    int intLocation{ glGetUniformLocation(shader.getID(), INT_UNIFORM) };
    int floatLocation{ glGetUniformLocation(shader.getID(), FLOAT_UNIFORM) };
    int float2Location{ glGetUniformLocation(shader.getID(), FLOAT_2_UNIFORM) };
    int float3Location{ glGetUniformLocation(shader.getID(), FLOAT_3_UNIFORM) };
    int float4Location{ glGetUniformLocation(shader.getID(), FLOAT_4_UNIFORM) };
    int matrix4Location{ glGetUniformLocation(shader.getID(), MATRIX_4_UNIFORM) };


    /// \brief Test the \ref Shader RAII behavior.
    ///
    /// The constructor should create a valid OpenGL shader program and the destructor should delete the program.
    SECTION("Shader RAII")
    {
        unsigned int originalID{};

        {
            const Shader original(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC, GEOMETRY_SHADER_SRC);
            originalID = original.getID();

            CHECK(originalID != 0);
            CHECK(static_cast<bool>(glIsProgram(originalID)));
        }

        CHECK(!static_cast<bool>(glIsProgram(originalID)));
    }

    /// \brief Test the \ref Shader move constructor.
    ///
    /// Move constructing a \ref Shader should preserve the underlying shader program ID.
    SECTION("Shader move constructor")
    {
        Shader original(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
        const unsigned int originalID{ original.getID() };
        Shader moved{ std::move(original) };

        CHECK(moved.getID() == originalID);
        CHECK(static_cast<bool>(glIsProgram(moved.getID())));
    }

    /// \brief Test the \ref Shader move assignment.
    ///
    /// Move assignment should preserve the move source's shader program and clean up the resource held in the move target
    /// before assigning the move source to it.
    SECTION("Shader move assignment")
    {
        Shader shader1(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
        const unsigned int originalShader1ID{ shader1.getID() };
        Shader shader2(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
        const unsigned int originalShader2ID{ shader2.getID() };

        shader2 = std::move(shader1);

        CHECK(!static_cast<bool>(glIsProgram(originalShader2ID)));
        CHECK(shader2.getID() == originalShader1ID);
        CHECK(static_cast<bool>(glIsProgram(shader2.getID())));
    }

    /// \brief Test the \ref Shader move assignment when assigning to the same object.
    ///
    /// When the move target is the same as the move source, nothing should change.
    SECTION("Shader move assignment on same shader")
    {
        Shader shader(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC);
        const unsigned int originalShaderID{ shader.getID() };

        shader = std::move(shader);

        CHECK(static_cast<bool>(glIsProgram(originalShaderID)));
        CHECK(shader.getID() == originalShaderID);
    }

    /// \brief Test the \ref Shader use method.
    ///
    /// When the shader is being activated with the use method, OpenGL should report it back as the currently used program.
    SECTION("Shader use activates program")
    {
        shader.use();

        int currentProgram{ 0 };
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

        CHECK(currentProgram != 0);
        CHECK(shader.getID() == currentProgram);
    }

    /// \brief Test uploading a single float uniform value
    ///
    /// When a value is uploaded to a uniform, the shader should receive the uploaded value
    SECTION("Set single floating value with use")
    {
        const auto val{ generateRandomValue<float>() };
        shader.setFloat(FLOAT_UNIFORM, val, true);

        GLfloat uniformValue{ 0 };
        getUniformValue<GLfloat>(shader, floatLocation, uniformValue);

        CHECK(uniformValue == val);
    }

    /// \brief Test uploading a single integer uniform value
    ///
    /// When a value is uploaded to a uniform, the shader should receive the uploaded value
    SECTION("Set single integer value with use")
    {
        const auto val{ generateRandomValue<int>() };
        shader.setInteger(INT_UNIFORM, val, true);

        GLint uniformValue{ 0 };
        getUniformValue<GLint>(shader, intLocation, uniformValue);

        CHECK(uniformValue == val);
    }

    /// \brief Test uploading a 2 element float vector uniform value using 2 individual values
    ///
    /// When a value is uploaded to a uniform, the shader should receive the uploaded value
    SECTION("Set two single float values with use")
    {
        const std::array original{ generateRandomValue<float>(), generateRandomValue<float>() };

        shader.setVector2f(FLOAT_2_UNIFORM, original[0], original[1], true);

        std::array<GLfloat, original.size()> uniformValues{};
        getUniformValue<GLfloat>(shader, float2Location, uniformValues);

        CHECK(uniformValues == original);
    }

    /// \brief Test uploading a 2 element float vector uniform value using a glm::vec2
    ///
    /// When a value is uploaded to a uniform, the shader should receive the uploaded value
    SECTION("Set float vector2 value with use")
    {
        const auto vec{ glm::vec2(generateRandomValue<float>(), generateRandomValue<float>()) };

        shader.setVector2f(FLOAT_2_UNIFORM, vec, true);

        std::array<GLfloat, glm::vec2::length()> uniformValues{};
        getUniformValue<GLfloat>(shader, float2Location, uniformValues);

        CHECK(uniformValues.at(0) == vec.x);
        CHECK(uniformValues.at(1) == vec.y);
    }

    /// \brief Test uploading a 3 element float vector uniform value using 3 individual values
    ///
    /// When a value is uploaded to a uniform, the shader should receive the uploaded value
    SECTION("Set three single float values with use")
    {
        const std::array original{ generateRandomValue<float>(),
                                   generateRandomValue<float>(),
                                   generateRandomValue<float>() };

        shader.setVector3f(FLOAT_3_UNIFORM, original[0], original[1], original[2], true);

        std::array<GLfloat, original.size()> uniformValues{};
        glGetUniformfv(shader.getID(), float3Location, uniformValues.data());

        CHECK(uniformValues == original);
    }

    /// \brief Test uploading a 3 element float vector uniform value using a glm::vec3
    ///
    /// When a value is uploaded to a uniform, the shader should receive the uploaded value
    SECTION("Set float vector3 value with use")
    {
        const auto vec{
            glm::vec3(generateRandomValue<float>(), generateRandomValue<float>(), generateRandomValue<float>())
        };

        shader.setVector3f(FLOAT_3_UNIFORM, vec, true);

        std::array<GLfloat, glm::vec3::length()> uniformValues{};
        glGetUniformfv(shader.getID(), float3Location, uniformValues.data());

        CHECK(uniformValues.at(0) == vec.x);
        CHECK(uniformValues.at(1) == vec.y);
        CHECK(uniformValues.at(2) == vec.z);
    }

    /// \brief Test uploading a 4 element float vector uniform value using 4 individual values
    ///
    /// When a value is uploaded to a uniform, the shader should receive the uploaded value
    SECTION("Set four single float values with use")
    {
        const std::array original{ generateRandomValue<float>(),
                                   generateRandomValue<float>(),
                                   generateRandomValue<float>(),
                                   generateRandomValue<float>() };

        // NOLINTNEXTLINE(readability-magic-numbers): 3 used as array index
        shader.setVector4f(FLOAT_4_UNIFORM, original[0], original[1], original[2], original[3], true);

        std::array<GLfloat, original.size()> uniformValues{};
        glGetUniformfv(shader.getID(), float4Location, uniformValues.data());

        CHECK(uniformValues == original);
    }

    /// \brief Test uploading a 4 element float vector uniform value using a single glm::vec4
    ///
    /// When a value is uploaded to a uniform, the shader should receive the uploaded value
    SECTION("Set float vector4 value with use")
    {
        const auto vec{ glm::vec4(
            generateRandomValue<float>(),
            generateRandomValue<float>(),
            generateRandomValue<float>(),
            generateRandomValue<float>()
        ) };

        shader.setVector4f(FLOAT_4_UNIFORM, vec, true);

        std::array<GLfloat, glm::vec4::length()> uniformValues{};
        glGetUniformfv(shader.getID(), float4Location, uniformValues.data());

        CHECK(uniformValues.at(0) == vec.x);
        CHECK(uniformValues.at(1) == vec.y);
        CHECK(uniformValues.at(2) == vec.z);
        CHECK(uniformValues.at(3) == vec.w);
    }

    /// \brief Test uploading a 4x4 float matrix uniform value
    ///
    /// When a value is uploaded to a uniform, the shader should receive the uploaded value
    SECTION("Set matrix4 value with use")
    {
        const auto mat{ glm::mat4(generateRandomValue<float>()) };

        shader.setMatrix4(MATRIX_4_UNIFORM, mat, true);

        std::array<GLfloat, static_cast<std::size_t>(glm::mat4::length() * glm::mat4::length())> uniformValues{};
        getUniformValue<GLfloat>(shader, matrix4Location, uniformValues);

        const glm::mat4 result{ glm::make_mat4(uniformValues.data()) };
        CHECK(result == mat);
    }

    glContext.teardown();
}

} // namespace sfa::testing

