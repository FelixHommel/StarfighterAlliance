#ifndef SFA_SRC_ENGINE_CORE_SHADER_HPP
#define SFA_SRC_ENGINE_CORE_SHADER_HPP

#include "glm/ext/matrix_float4x4.hpp"
#include "glad/gl.h"

#include <cstdint>
#include <string_view>

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
    /// \param vertSource the source code of the vertex shader
    /// \param fragSource the source code of the fragment shader
    /// \param geomSource(optional) the source code of the geometry shader
    Shader(const char* vertSource, const char* fragSource, const char* geomSource = nullptr);
    ~Shader();

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void use() const;
    [[nodiscard]] unsigned int getID() const noexcept { return m_id; }

    /// \brief Set a single float uniform value.
    void setFloat(const char* name, float value, bool useShader = false) const;
    /// \brief Set a single integer uniform value.
    void setInteger(const char* name, int value, bool useShader = false) const;
    /// \brief Set a 2-element float vector uniform value.
    void setVector2f(const char* name, float x, float y, bool useShader = false) const;
    /// \brief Set a 2-element float vector uniform value.
    void setVector2f(const char* name, const glm::vec2& value, bool useShader = false) const;
    /// \brief Set a 3-element float vector uniform value.
    void setVector3f(const char* name, float x, float y, float z, bool useShader = false) const;
    /// \brief Set a 3-element float vector uniform value.
    void setVector3f(const char* name, const glm::vec3& value, bool useShader = false) const;
    /// \brief Set a 4-element float vector uniform value.
    void setVector4f(const char* name, float x, float y, float z, float w, bool useShader = false) const;
    /// \brief Set a 4-element float vector uniform value.
    void setVector4f(const char* name, const glm::vec4& value, bool useShader = false) const;
    /// \brief Set a 4x4 float matrix uniform value.
    void setMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false) const;

private:
    static constexpr int ERROR_LOG_SIZE{ 1024 };

    /// \brief Helper enum to describe what type of OpenGL object was compiled/linked
    enum class CompilationType : std::uint8_t
    {
        Vertex,
        Fragment,
        Geometry,
        Program
    };

    unsigned int m_id{ 0 };

    void releaseShaderProgram() const;

    static unsigned int compileShader(const char* source, CompilationType type);
    static void checkCompileErrors(unsigned int object, CompilationType type);
    static std::string_view compilationTypeToString(CompilationType type);
    static GLenum compilationTypeToGlenum(CompilationType type);
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_CORE_SHADER_HPP

