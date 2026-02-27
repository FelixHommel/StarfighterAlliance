#include "ResourceContext.hpp"

#include "core/Shader.hpp"
#include "core/Texture.hpp"
#include "core/resourceManagement/IResourceLoader.hpp"
#include "core/resourceManagement/IntermediateResourceData.hpp"
#include "core/resourceManagement/ResourceLoader.hpp"

#include <spdlog/spdlog.h>

#include <atomic>
#include <cstddef>
#include <exception>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <variant>

namespace sfa
{

ResourceContext::ResourceContext(std::unique_ptr<IResourceLoader> loader)
    : m_loader((loader != nullptr) ? std::move(loader) : std::make_unique<ResourceLoader>())
{}

ResourceContext::~ResourceContext()
{
    m_threadPool->shutdown(true);
    m_uploadQueue.close();
}

void ResourceContext::requestResource(const ResourceRequest& request)
{
    m_inFlight.fetch_add(1, std::memory_order_acq_rel);
    std::visit([this](const auto& req) { this->enqueueLoadTask(req); }, request);
}

void ResourceContext::processUploadQueue(std::size_t maxUploads)
{
    std::size_t processed{ 0 };
    while(maxUploads == 0 || processed < maxUploads)
    {
        if(const auto& task{ m_uploadQueue.tryPop() }; task.has_value())
        {
            processUploadTask(*task);

            m_inFlight.fetch_sub(1, std::memory_order_acq_rel);
            ++processed;
        }
        else
            break;
    }
}

void ResourceContext::waitForAllUploads()
{
    while(m_inFlight.load(std::memory_order_acquire) > 0)
    {
        processUploadQueue();
        std::this_thread::yield();
    }
}

void ResourceContext::clear()
{
    m_shaderCache.clear();
    m_textureCache.clear();
}

/// \brief Enqueue a new \ref ShaderLoadRequest.
///
/// \param request providing details about the shader that is to be loaded.
void ResourceContext::enqueueLoadTask(const ShaderLoadRequest& request)
{
    m_threadPool->enqueue([this, req = request]() {
        m_uploadQueue.emplace(req.name, m_loader->loadShader(req.vert, req.frag, req.geom));
    });
}

/// \brief Enqueue a new \ref TextureLoadRequest.
///
/// \param request providing details about the texture that is to be loaded.
void ResourceContext::enqueueLoadTask(const TextureLoadRequest& request)
{
    m_threadPool->enqueue([this, req = request]() {
        m_uploadQueue.emplace(req.name, m_loader->loadTexture(req.filepath));
    });
}

/// \brief Upload a loaded resource to the GPU.
///
/// \param task a \ref UploadTask containing the needed information to upload the resource.
void ResourceContext::processUploadTask(const UploadTask& task)
{
    if(const auto t{ task.result }; t.has_value())
        std::visit([this, &task](const auto& resourceData) { this->uploadToGPU(task.key, resourceData); }, t.value());
    else
    {
        const auto& error{ t.error() };
        spdlog::error("Failed to load resource '{}': {} ({})", task.key, error.message, error.filepath.string());

        // TODO: Potentially store error in the resource cache, or give other possibility for accessors to know the resource
        // failed loading.
        return;
    }
}

/// \brief Upload a shader to the GPU
///
/// \param key with which the shader can be accessed
/// \param data \ref ShaderSourceData containing the source code for the shader
void ResourceContext::uploadToGPU(const std::string& key, const ShaderSourceData& data)
{
    try
    {
        m_shaderCache.store(
            key,
            std::make_shared<Shader>(
                data.vertexSource.c_str(),
                data.fragmentSource.c_str(),
                data.geometrySource.empty() ? nullptr : data.geometrySource.c_str()
            )
        );

        spdlog::info("Upload shader '{}' to GPU", key);
    }
    catch(const std::exception& e)
    {
        spdlog::error("Failed to upload shader '{}': {}", key, e.what());
    }
}

/// \brief Upload a texture to the GPU
///
/// \param key with which the texture can be accessed
/// \param data \ref TextureRawData containing the pixel data for the texture
void ResourceContext::uploadToGPU(const std::string& key, const TextureRawData& data)
{
    try
    {
        m_textureCache.store(key, std::make_shared<Texture2D>(data.width, data.height, data.channels, data.pixels));

        spdlog::info("Upload texture '{}' to GPU", key);
    }
    catch(const std::exception& e)
    {
        spdlog::error("Failed to upload texture '{}': {}", key, e.what());
    }
}

} // namespace sfa

