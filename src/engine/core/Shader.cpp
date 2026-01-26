#include "Shader.hpp"

#include "glad/gl.h"
#include "glm/gtc/type_ptr.hpp"
#include "spdlog/spdlog.h"

#include <array>
#include <cassert>
#include <string_view>
#include <utility>

namespace sfa
{

Shader::Shader(const char* vertSource, const char* fragSource, const char* geomSource) : m_id{ glCreateProgram() }
{
    const unsigned int vertId{ compileShader(vertSource, CompilationType::Vertex) };
    const unsigned int fragId{ compileShader(fragSource, CompilationType::Fragment) };
    const unsigned int geomId{ compileShader(geomSource, CompilationType::Geometry) };

    glAttachShader(m_id, vertId);
    glAttachShader(m_id, fragId);
    if(geomSource != nullptr)
        glAttachShader(m_id, geomId);

    glLinkProgram(m_id);
    checkCompileErrors(m_id, CompilationType::Program);

    glDeleteShader(vertId);
    glDeleteShader(fragId);
    if(geomSource != nullptr)
        glDeleteShader(geomId);
}

Shader::~Shader()
{
    releaseShaderProgram();
}

Shader::Shader(Shader&& other) noexcept
    : m_id(std::exchange(other.m_id, 0))
{}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if(this == &other)
        return *this;
    
    releaseShaderProgram();

    m_id = std::exchange(other.m_id, 0);

    return *this;
}

void Shader::use() const
{
    glUseProgram(m_id);
}

void Shader::setFloat(const char* name, float value, bool useShader) const
{
    if(useShader)
        use();
    glUniform1f(glGetUniformLocation(m_id, name), value);
}
void Shader::setInteger(const char* name, int value, bool useShader) const
{
    if(useShader)
        use();
    glUniform1i(glGetUniformLocation(m_id, name), value);
}
void Shader::setVector2f(const char* name, float x, float y, bool useShader) const
{
    if(useShader)
        use();
    glUniform2f(glGetUniformLocation(m_id, name), x, y);
}
void Shader::setVector2f(const char* name, const glm::vec2& value, bool useShader) const
{
    if(useShader)
        use();
    glUniform2f(glGetUniformLocation(m_id, name), value.x, value.y);
}
void Shader::setVector3f(const char* name, float x, float y, float z, bool useShader) const
{
    if(useShader)
        use();
    glUniform3f(glGetUniformLocation(m_id, name), x, y, z);
}
void Shader::setVector3f(const char* name, const glm::vec3& value, bool useShader) const
{
    if(useShader)
        use();
    glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
}
void Shader::setVector4f(const char* name, float x, float y, float z, float w, bool useShader) const
{
    if(useShader)
        use();
    glUniform4f(glGetUniformLocation(m_id, name), x, y, z, w);
}
void Shader::setVector4f(const char* name, const glm::vec4& value, bool useShader) const
{
    if(useShader)
        use();
    glUniform4f(glGetUniformLocation(m_id, name), value.x, value.y, value.z, value.w);
}
void Shader::setMatrix4(const char* name, const glm::mat4& matrix, bool useShader) const
{
    if(useShader)
        use();
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, 0, glm::value_ptr(matrix));
}

/// \brief Delete the shader program stored in this shader.
void Shader::releaseShaderProgram() const
{
    if(m_id != 0)
        glDeleteProgram(m_id);
}

/// \brief Compile a shader.
///
/// \param source the source code of the shader
/// \param type what kind of shader it is
///
/// \returns the ID of the created shader
unsigned int Shader::compileShader(const char* source, CompilationType type)
{
#ifdef SFA_DEBUG
    assert(type != CompilationType::Program && "Program is not a compilable type");
#endif

    if(source == nullptr)
    {
        spdlog::warn("No source was provided for {} shader", compilationTypeToString(type));
        return 0;
    }

    unsigned int id{ glCreateShader(compilationTypeToGlenum(type)) };
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    checkCompileErrors(id, type);

    return id;
}

/// \brief Check if there was a compilation error in an OpenGL Shader or Program.
///
/// \param object the OpenGL object to be checked (shader or program)
/// \param type type of the OpenGL object that was compiled
void Shader::checkCompileErrors(unsigned int object, CompilationType type)
{
    int success{ 0 };
    std::array<char, ERROR_LOG_SIZE> infoLog{};
    if(type != CompilationType::Program)
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if(success == 0)
        {
            glGetShaderInfoLog(object, infoLog.size(), nullptr, infoLog.data());
            spdlog::error("Shader compile-error({}):\n{}", compilationTypeToString(type), infoLog.data());
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if(success == 0)
        {
            glGetProgramInfoLog(object, infoLog.size(), nullptr, infoLog.data());
            spdlog::error("Program link-error({}):\n{}", compilationTypeToString(type), infoLog.data());
        }
    }
}

/// \brief Convert a \ref CompilationType to string
///
/// \param type \ref CompilationType that is being converted
///
/// \returns string representation of \p type
std::string_view Shader::compilationTypeToString(CompilationType type)
{
    switch(type)
    {
        case CompilationType::Vertex: return "Vertex";
        case CompilationType::Fragment: return "Fragment";
        case CompilationType::Geometry: return "Geometry";
        case CompilationType::Program: return "Program";
    }
}

/// \brief Convert a \ref CompilationType to Glenum
///
/// \param type \ref CompilationType that is being converted
///
/// \returns GLenum representation of \p type
GLenum Shader::compilationTypeToGlenum(CompilationType type)
{
    switch(type)
    {
        case CompilationType::Vertex: return GL_VERTEX_SHADER;
        case CompilationType::Fragment: return GL_FRAGMENT_SHADER;
        case CompilationType::Geometry: return GL_GEOMETRY_SHADER;
        case CompilationType::Program: return GL_PROGRAM;
    }
}

} // namespace sfa

