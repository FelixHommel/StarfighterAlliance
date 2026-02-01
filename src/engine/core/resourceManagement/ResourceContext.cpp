#include "ResourceContext.hpp"

#include "core/Shader.hpp"
#include "core/Texture.hpp"

#include <spdlog/spdlog.h>
#include <stb_image.h>

#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <memory>
#include <span>
#include <sstream>
#include <string>

namespace sfa
{

void ResourceContext::loadShaderFromFile(
    const std::string& name,
    const std::filesystem::path& vert,
    const std::filesystem::path& frag,
    const std::filesystem::path& geom
)
{
    std::string vertexShaderCode;
    std::string fragmentShaderCode;
    std::string geometryShaderCode;

    try
    {
        std::ifstream vertexShaderFile{ vert };
        std::ifstream fargmentShaderFile{ frag };
        std::ifstream geometryShaderFile{ geom };

        std::stringstream shaderCode;
        shaderCode << vertexShaderFile.rdbuf();
        vertexShaderCode = shaderCode.str();

        shaderCode.clear();
        shaderCode << fargmentShaderFile.rdbuf();
        fragmentShaderCode = shaderCode.str();

        shaderCode.clear();
        shaderCode << geometryShaderFile.rdbuf();
        geometryShaderCode = shaderCode.str();
    }
    catch(const std::exception& e)
    {
        spdlog::error(
            "Failed to read source files for {} shader at:\n\t- {}\n\t- {}\n\t- {}",
            name,
            vert.string(),
            frag.string(),
            geom.string()
        );
    }

    m_shaderCache.store(
        name, std::make_shared<Shader>(vertexShaderCode.c_str(), fragmentShaderCode.c_str(), geometryShaderCode.c_str())
    );
}

void ResourceContext::loadTextureFromFile(std::string name, const std::filesystem::path& filepath)
{
    m_textureCache.store(name, std::make_shared<Texture2D>(loadTexture(filepath)));
}

void ResourceContext::clear()
{
    m_shaderCache.clear();
    m_textureCache.clear();
}

// Shader ResourceContext::loadShader(const std::filesystem::path& vert, const std::filesystem::path& frag, const std::filesystem::path& geom)
// {
// }

Texture2D ResourceContext::loadTexture(const std::filesystem::path& filepath)
{
    int width{};
    int height{};
    int nrChannels{};
    const auto u8path{ filepath.u8string() };
    stbi_uc* data{ stbi_load(reinterpret_cast<const char*>(u8path.c_str()), &width, &height, &nrChannels, 0) };

    const auto size{ static_cast<std::size_t>(width * height) * 4 };
    std::span<const std::byte> pixels{ reinterpret_cast<const std::byte*>(data), size };

    Texture2D tex(width, height, nrChannels, pixels);

    stbi_image_free(data);

    return tex;
}

} // namespace sfa

