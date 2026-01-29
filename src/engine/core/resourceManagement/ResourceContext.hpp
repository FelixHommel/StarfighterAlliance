#ifndef SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_CONTEXT_HPP
#define SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_CONTEXT_HPP

#include "core/Shader.hpp"
#include "core/Texture.hpp"
#include "core/resourceManagement/ResourceCache.hpp"

#include <filesystem>
#include <memory>
#include <string>

namespace sfa
{

/// \brief The \ref ResourceContext class is responsible for loading and managing resources.
///
/// \author Felix Hommel
/// \date 1/23/2026
class ResourceContext
{
public:
    ResourceContext() = default;
    ~ResourceContext() = default;

    ResourceContext(const ResourceContext&) = delete;
    ResourceContext(ResourceContext&&) = delete;
    ResourceContext& operator=(const ResourceContext&) = delete;
    ResourceContext& operator=(ResourceContext&&) = delete;

    void loadShaderFromFile(const std::string& name, const std::filesystem::path& vert, const std::filesystem::path& frag);
    void loadShaderFromFile(const std::string& name, const std::filesystem::path& vert, const std::filesystem::path& frag, const std::filesystem::path& geom);
    std::shared_ptr<Shader> getShader(const std::string& key) { return m_shaderCache.get(key); }

    void loadTextureFromFile(std::string name, const std::filesystem::path& filepath);
    std::shared_ptr<Texture2D> getTexture(const std::string& key) { return m_textureCache.get(key); }

    void clear();

private:
    ResourceCache<Shader> m_shaderCache;
    ResourceCache<Texture2D> m_textureCache;

    // static Shader loadShader(const std::filesystem::path& vert, const std::filesystem::path& frag);
    // static Shader loadShader(const std::filesystem::path& vert, const std::filesystem::path& frag, const std::filesystem::path& geom);
    static Texture2D loadTexture(const std::filesystem::path& filepath);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_CONTEXT_HPP

