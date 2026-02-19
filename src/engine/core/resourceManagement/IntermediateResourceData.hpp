#ifndef SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_INTERMEDIATE_RESOURCE_DATA_HPP
#define SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_INTERMEDIATE_RESOURCE_DATA_HPP

#include "core/resourceManagement/ResourceError.hpp"

#include <cstddef>
#include <expected>
#include <string>
#include <variant>
#include <vector>

namespace sfa
{

/// \brief Information from which an OpenGL shader program can be created.
///
/// \author Felix Hommel
/// \date 2/10/2026
struct ShaderSourceData
{
    std::string vertexSource;
    std::string fragmentSource;
    std::string geometrySource; ///< Empty string if shader does not use geometry shaders
};

/// \brief Information from which an OpenGL texture can be created.
///
/// \author Felix Hommel
/// \date 2/10/2026
struct TextureRawData
{
    int width;
    int height;
    int channels;
    std::vector<std::byte> pixels;
};

using ResourceData = std::variant<ShaderSourceData, TextureRawData>;

using LoadResult = std::expected<ResourceData, ResourceError>;

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_INTERMEDIATE_RESOURCE_DATA_HPP

