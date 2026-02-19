#ifndef SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_LOADER_HPP
#define SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_LOADER_HPP

#include "core/resourceManagement/IResourceLoader.hpp"
#include "core/resourceManagement/IntermediateResourceData.hpp"
#include "core/resourceManagement/ResourceError.hpp"

#include <expected>
#include <filesystem>
#include <string>

namespace sfa
{

/// \brief Load resource data from the disk.
///
/// \author Felix Hommel
/// \date 2/14/2026
class ResourceLoader : public IResourceLoader
{
public:
    LoadResult loadShader(
        const std::filesystem::path& vert,
        const std::filesystem::path& frag,
        const std::filesystem::path& geom = std::filesystem::path("")
    ) override;
    LoadResult loadTexture(const std::filesystem::path& filepath) override;

private:
    static std::expected<std::string, ResourceError> readFile(const std::filesystem::path& filepath);
};

} // namespace sfa

#endif // ! SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_LOADER_HPP

