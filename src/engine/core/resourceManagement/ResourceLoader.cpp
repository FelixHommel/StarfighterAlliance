#include "ResourceLoader.hpp"

#include "core/resourceManagement/IntermediateResourceData.hpp"
#include "core/resourceManagement/ResourceError.hpp"

#include <fmt/format.h>
#include <stb_image.h>

#include <cstddef>
#include <cstring>
#include <expected>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

namespace sfa
{

LoadResult ResourceLoader::loadShader(
    const std::filesystem::path& vert, const std::filesystem::path& frag, const std::filesystem::path& geom
)
{
    ShaderSourceData data;

    auto result{ readFile(vert) };
    if(!result)
        return std::unexpected(result.error());
    data.vertexSource = std::move(*result);

    result = readFile(frag);
    if(!result)
        return std::unexpected(result.error());
    data.fragmentSource = std::move(*result);

    if(!geom.empty())
    {
        result = readFile(geom);
        if(!result)
            return std::unexpected(result.error());
        data.geometrySource = std::move(*result);
    }

    return ResourceData{ std::move(data) };
}

LoadResult ResourceLoader::loadTexture(const std::filesystem::path& filepath)
{
    if(!std::filesystem::exists(filepath))
        return std::unexpected(ResourceError::fileNotFound(filepath));

    int width{};
    int height{};
    int nrChannels{};
    // NOTE: Need to access filepath as u8string to ensure windows file paths are accessed in the right way
    stbi_uc* imageData{ stbi_load(reinterpret_cast<const char*>(filepath.u8string().c_str()), &width, &height, &nrChannels, 0) };

    if(imageData == nullptr)
        return std::unexpected(
            ResourceError::invalidFormat(filepath, fmt::format("stb_image failed: {}", stbi_failure_reason()))
        );

    const auto size{ static_cast<std::size_t>(width * height * nrChannels) };
    std::vector<std::byte> pixels(size);
    std::memcpy(pixels.data(), imageData, size);
    stbi_image_free(imageData);

    TextureRawData textureData{ .width = width, .height = height, .channels = nrChannels, .pixels = std::move(pixels) };

    return ResourceData{ std::move(textureData) };
}

/// \brief Load a file to a std::string.
///
/// \param filepath path to the file
///
/// \returns a std::expected with the file content as string, or a \ref ResourceError if something went wrong while loading
std::expected<std::string, ResourceError> ResourceLoader::readFile(const std::filesystem::path& filepath)
{
    if(!std::filesystem::exists(filepath))
        return std::unexpected(ResourceError::fileNotFound(filepath));

    std::ifstream file(filepath);
    if(!file)
        return std::unexpected(ResourceError::invalidFormat(filepath, "Could not open the file"));

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

} // namespace sfa

