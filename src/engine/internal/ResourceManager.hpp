#ifndef INTERNAL_RESOURCE_MANAGER_HPP
#define INTERNAL_RESOURCE_MANAGER_HPP

#include "Shader.hpp"
#include "Texture.hpp"

#include <map>
#include <string>

/**
 * @file ResourceManager.hpp
 * @brief The ResourceManager holds all Textues and Shaders
 *
 * @details A singleton class that manages and holds all Textures and Shaders. Through the singleton
 *          behaviour all loaded Textures and Shaders can be accesed from all calsses taht need access.
 *
 * @author Felix Hommel
 * @date Nov 16, 2024
 */
class ResourceManager
{
public:
    ResourceManager() = delete;

    /** Loading and accessing Shaders */
    static Shader loadShader(
        const char* vertexShaderFilepath, const char* fShaderFile, const char* gShaderFile, const std::string& name
    );
    static Shader& getShader(const std::string& name);

    /** Loading and accessing Textures */
    static Texture2D loadTexture(const char* filepath, const std::string& name);
    static Texture2D& getTexture(const std::string& name);

    /** Clearing all resources */
    static void clear();

private:
    /** Private members */
    static std::map<std::string, Shader> m_shaders;
    static std::map<std::string, Texture2D> m_textures;

    /** Private member functions */
    static Shader loadShaderFromFile(
        const char* vertexShaderFilepath, const char* fShaderFile, const char* gShaderFile = nullptr
    );
    static Texture2D loadTextureFromFile(const char* filepath);
};

#endif //! INTERNAL_RESOURCE_MANAGER_HPP
