#ifndef SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_I_RESOURCE_LOADER_HPP
#define SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_I_RESOURCE_LOADER_HPP

#include "core/resourceManagement/IntermediateResourceData.hpp"

#include <filesystem>

namespace sfa
{

/// \brief Interface for resource loading components.
///
/// Loading a resource in this context means access the resource on the disk and store it in a format that can later
/// be used to register the resource with OpenGL and upload it to the GPU.
///
/// \author Felix Hommel
/// \date 2/14/2026
class IResourceLoader
{
public:
    IResourceLoader() = default;
    virtual ~IResourceLoader() = default;

    IResourceLoader(const IResourceLoader&) = default;
    IResourceLoader& operator=(const IResourceLoader&) = default;
    IResourceLoader(IResourceLoader&&) noexcept = default;
    IResourceLoader& operator=(IResourceLoader&&) noexcept = default;

    /// \brief Load a shader from the disk.
    ///
    /// An OpenGL shader program consists of a vertex, fragment, and optionally a geometry shader. The shader source
    /// code is loaded by this function.
    ///
    /// \param vert path to the vertex shader file
    /// \param frag path to the fragment shader file
    /// \param geom (optional) path to the geometry shader file
    ///
    /// \returns \ref LoadResult of the load attempt
    virtual LoadResult loadShader(
        const std::filesystem::path& vert,
        const std::filesystem::path& frag,
        const std::filesystem::path& geom = std::filesystem::path("")
    ) = 0;
    /// \brief Load a texture from the disk.
    ///
    /// A texture can be any PNG or JPEG image.
    ///
    /// \param filepath path to the texture file
    ///
    /// \returns \ref LoadResult of the load attempt
    virtual LoadResult loadTexture(const std::filesystem::path& filepath) = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_I_RESOURCE_LOADER_HPP

