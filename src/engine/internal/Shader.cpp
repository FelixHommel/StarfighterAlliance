#include "Shader.hpp"

#include "glad/gl.h"
#include "glm/gtc/type_ptr.hpp"

#include <array>
#include <iostream>

/*
 * @brief Compile the Shader code to a OpenGL Shader
 *
 * @param vertexSource, the source code of the vertex shader
 * @param fragmentSource, the source code of the fragment shader
 * @param geometrySource, the sourcec code of the geometry shader. Can be nullptr
*/
Shader::Shader(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    unsigned int sVertex{ 0 }; 
    unsigned int sFragment{ 0 };
    unsigned int gShader{ 0 };

    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, nullptr);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");

    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, nullptr);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");

    if (geometrySource != nullptr)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, nullptr);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, sVertex);
    glAttachShader(m_id, sFragment);
    if (geometrySource != nullptr)
        glAttachShader(m_id, gShader);
    glLinkProgram(m_id);
    checkCompileErrors(m_id, "PROGRAM");

    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr)
        glDeleteShader(gShader);
}

/*
 * @brief Set this shader as active shader
*/
Shader& Shader::use()
{
    glUseProgram(m_id);
    return *this;
}

/*
 * @brief Compile the Shader code to a OpenGL Shader. This function is used if the Shader was created with the empty constructor
 *
 * @param vertexSource, the source code of the vertex shader
 * @param fragmentSource, the source code of the fragment shader
 * @param geometrySource, the sourcec code of the geometry shader. Can be nullptr
*/
void Shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    unsigned int sVertex{ 0 }; 
    unsigned int sFragment{ 0 };
    unsigned int gShader{ 0 };

    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, nullptr);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");

    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, nullptr);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");

    if (geometrySource != nullptr)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, nullptr);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, sVertex);
    glAttachShader(m_id, sFragment);
    if (geometrySource != nullptr)
        glAttachShader(m_id, gShader);
    glLinkProgram(m_id);
    checkCompileErrors(m_id, "PROGRAM");

    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr)
        glDeleteShader(gShader);
}

/*
 * All of the following functions are used to set uniform values in shaders
 */
void Shader::setFloat(const char* name, float value, bool useShader)
{
    if (useShader)
        use();
    glUniform1f(glGetUniformLocation(m_id, name), value);
}
void Shader::setInteger(const char* name, int value, bool useShader)
{
    if (useShader)
        use();
    glUniform1i(glGetUniformLocation(m_id, name), value);
}
void Shader::setVector2f(const char* name, float x, float y, bool useShader)
{
    if (useShader)
        use();
    glUniform2f(glGetUniformLocation(m_id, name), x, y);
}
void Shader::setVector2f(const char* name, const glm::vec2& value, bool useShader)
{
    if (useShader)
        use();
    glUniform2f(glGetUniformLocation(m_id, name), value.x, value.y);
}
void Shader::setVector3f(const char* name, float x, float y, float z, bool useShader)
{
    if (useShader)
        use();
    glUniform3f(glGetUniformLocation(m_id, name), x, y, z);
}
void Shader::setVector3f(const char* name, const glm::vec3& value, bool useShader)
{
    if (useShader)
        use();
    glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
}
void Shader::setVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
    if (useShader)
        use();
    glUniform4f(glGetUniformLocation(m_id, name), x, y, z, w);
}
void Shader::setVector4f(const char* name, const glm::vec4& value, bool useShader)
{
    if (useShader)
        use();
    glUniform4f(glGetUniformLocation(m_id, name), value.x, value.y, value.z, value.w);
}
void Shader::setMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
    if (useShader)
        use();
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, 0, glm::value_ptr(matrix));
}

/*
 * @brief Check if there was a compilation error in an OpenGL Shader or Program
 *
 * @param object, the OpenGL object to be checked (can be Shader or Program)
 * @param type, type of the Object. Can be "PROGRAM", "VERTEX", "FRAGMENT", "GEOMETRY"
*/
void Shader::checkCompileErrors(unsigned int object, std::string type)
{
    int success{ 0 };
    const int logSize{ 1024 };
    std::array<char, logSize> infoLog{};
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (success == 0)
        {
            glGetShaderInfoLog(object, infoLog.size(), nullptr, infoLog.data());
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog.data() << "\n-------------------------------------------------------\n";
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (success == 0)
        {
            glGetProgramInfoLog(object, infoLog.size(), nullptr, infoLog.data());
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog.data() << "\n-------------------------------------------------------\n";
        }
    }
}
