#ifndef SFA_SRC_ENGINE_CORE_SHADER_HPP
#define SFA_SRC_ENGINE_CORE_SHADER_HPP

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

namespace sfa
{

/// \brief Abstraction of OpenGL shaders.
///
/// The \ref Shader class manages the program ID and provides utility to upload shader attributes.
///
/// \author Felix Hommel
/// \date 1/24/2026
class Shader
{
public:
    /// \brief Compile the Shader code to an OpenGL Shader.
    ///
    /// \param pVertSource the source code of the vertex shader
    /// \param pFragSource the source code of the fragment shader
    /// \param pGeomSource(optional) the source code of the geometry shader
    Shader(const char* pVertSource, const char* pFragSource, const char* pGeomSource = nullptr);
    ~Shader();

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void use() const;
    [[nodiscard]] GLuint getID() const noexcept { return m_id; }

    /// \brief Set a shader uniform value.
    ///
    /// \tparam T Type of the uniform value
    ///
    /// \param name The name of the uniform field
    /// \param value The value for the uniform
    /// \param (optional) useShader Flag that determines whether the shader needs to be activated before setting the uniform.
    template<typename T>
    void setUniform(const std::string& name, const T& value, bool useShader = false)
    {
        setUniformImpl(name, value, useShader);
    }

private:
    using UniformCache = std::unordered_map<std::string, GLint>;

    static constexpr int ERROR_LOG_SIZE{ 1024 };

    /// \brief Helper enum to describe what type of OpenGL object was compiled/linked
    enum class CompilationType : std::uint8_t
    {
        Vertex,
        Fragment,
        Geometry,
        Program
    };

    GLuint m_id{ 0 };
    UniformCache m_uniforms;

    void releaseShaderProgram() const;
    GLint getUniformLocation(const std::string& uniformName);

    void setUniformImpl(const std::string& name, float value, bool useShader);
    void setUniformImpl(const std::string& name, int value, bool useShader);
    void setUniformImpl(const std::string& name, const glm::vec2& value, bool useShader);
    void setUniformImpl(const std::string& name, const glm::vec3& value, bool useShader);
    void setUniformImpl(const std::string& name, const glm::vec4& value, bool useShader);
    void setUniformImpl(const std::string& name, const glm::mat4& matrix, bool useShader);

    static GLuint compileShader(const char* pSource, CompilationType type);
    static void checkCompileErrors(GLuint object, CompilationType type);
    static std::string_view compilationTypeToString(CompilationType type);
    static GLenum compilationTypeToGlenum(CompilationType type);
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_CORE_SHADER_HPP

