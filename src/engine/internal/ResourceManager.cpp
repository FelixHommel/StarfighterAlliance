#include "ResourceManager.hpp"

#include "glad/gl.h"
#include "external/stb_image.h"

#include <fstream>
#include <iostream>
#include <sstream>

/** Instantiate the map objects */
std::map<std::string, Texture2D> ResourceManager::m_textures;
std::map<std::string, Shader> ResourceManager::m_shaders;

/*
 * @brief load a new Shader from file
 *
 * @param vShaderFile, path to a vertex Shader
 * @param fShaderFile, path to a fragment Shader
 * @param gShaderFile, path to a geometry Shader
 * @param name, name of the Shader
 *
 * @return Shader
*/
Shader ResourceManager::loadShader(const char* vertexShaderFilepath, const char* fShaderFile, const char* gShaderFile, const std::string& name)
{
	m_shaders[name] = loadShaderFromFile(vertexShaderFilepath, fShaderFile, gShaderFile);
	return m_shaders[name];
}

/*
 * @brief return a Shader object
 *
 * @param name, name of the Shader to get
 *
 * @return Shader
*/
Shader& ResourceManager::getShader(const std::string& name)
{
	return m_shaders[name];
}

/*
 * @brief load a new Texture from file
 *
 * @param filepath, path to the Texture
 * @param name, name of the Texture
 *
 * @return Texture2D
*/
Texture2D ResourceManager::loadTexture(const char* filepath, const std::string& name)
{
	m_textures[name] = loadTextureFromFile(filepath);
	return m_textures[name];
}

/*
 * @brief return a Texture object
 *
 * @param name, name of the Texture to get
 *
 * @return Texture2D 
*/
Texture2D& ResourceManager::getTexture(const std::string& name)
{
	return m_textures[name];
}

/*
 * @brief delete all Texture and Shader objects currently stored
*/
void ResourceManager::clear()
{
	for(const auto& it: m_shaders)
		glDeleteProgram(it.second.getID());

	for(const auto& it: m_textures)
		glDeleteTextures(1, it.second.getID());
}

/*
 * @brief Read the Shadercode from file
*/
Shader ResourceManager::loadShaderFromFile(const char* vertexShaderFilepath, const char* fShaderFile, const char* gShaderFile)
{
	std::string vertexCode{};
	std::string fragmentCode{};
	std::string geometryCode{};

    try
    {
        std::ifstream vertexShaderFile{ vertexShaderFilepath };
        std::ifstream fragmentShaderFile{ fShaderFile };

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if(gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	return {vShaderCode, fShaderCode, ((gShaderFile != nullptr) ? gShaderCode : nullptr)};
}

/*
 * @brief Load the texture from file
*/
Texture2D ResourceManager::loadTextureFromFile(const char* filepath)
{
	Texture2D texture;

	int width {0};
	int height {0};
	int nrChannels {0};
	unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

	if(nrChannels == 4)
		texture.setRGBA();

	texture.generate(width, height, data);

	stbi_image_free(data);

	return texture;
}
