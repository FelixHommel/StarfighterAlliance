#ifndef SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_CONTEXT_HPP
#define SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_CONTEXT_HPP

#include "core/Shader.hpp"
#include "core/Texture.hpp"
#include "core/resourceManagement/IResourceLoader.hpp"
#include "core/resourceManagement/IntermediateResourceData.hpp"
#include "core/resourceManagement/ResourceCache.hpp"
#include "utility/BlockingQueue.hpp"
#include "utility/ThreadPool.hpp"

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <variant>

namespace sfa
{

/// \brief The \ref ResourceContext class is responsible for loading and managing resources.
///
/// \author Felix Hommel
/// \date 1/23/2026
class ResourceContext
{
public:
    /// \brief Containing information needed to load and upload a shader program.
    ///
    /// \author Felix Hommel
    /// \date 2/15/2026
    struct ShaderLoadRequest
    {
        std::string name;
        std::filesystem::path vert;
        std::filesystem::path frag;
        std::filesystem::path geom{ "" };
    };

    /// \brief Containing information needed to load and upload a texture.
    ///
    /// \author Felix Hommel
    /// \date 2/15/2026
    struct TextureLoadRequest
    {
        std::string name;
        std::filesystem::path filepath;
    };

    using ResourceRequest = std::variant<ShaderLoadRequest, TextureLoadRequest>;

    /// \brief Create a new \ref ResourceContext with a default \ref ResourceLoader for a loader.
    ///
    /// \param loader (optional) custom resource loader implementing \ref IResourceLoader
    explicit ResourceContext(std::unique_ptr<IResourceLoader> loader = nullptr);
    ~ResourceContext();

    ResourceContext(const ResourceContext&) = delete;
    ResourceContext& operator=(const ResourceContext&) = delete;
    ResourceContext(ResourceContext&&) noexcept = delete;
    ResourceContext& operator=(ResourceContext&&) noexcept = delete;

    /// \brief Request for a new resource to be uploaded.
    ///
    /// \param request providing details about the resource that is requested.
    void requestResource(const ResourceRequest& request);
    /// \brief Process pending GPU uploads.
    ///
    /// This method should only be called by the main OpenGL thread, to not interfere with OpenGL context boundaries.
    ///
    /// \param maxUploads (optional) specify a maximum number of queue elements to process (0 := unlimited)
    void processUploadQueue(std::size_t maxUploads = 0);

    void waitForAllUploads();

    [[nodiscard]] std::shared_ptr<Shader> getShader(const std::string& key) const { return m_shaderCache.get(key); }
    [[nodiscard]] std::shared_ptr<Texture2D> getTexture(const std::string& key) const
    {
        return m_textureCache.get(key);
    }
    [[nodiscard]] bool hasPendingUploads() const { return !m_uploadQueue.empty(); }
    [[nodiscard]] std::size_t pendingUploadTasks() const { return m_uploadQueue.size(); }
    [[nodiscard]] std::size_t totalResources() const noexcept { return m_shaderCache.size() + m_textureCache.size(); }

    void clear();

private:
    /// \brief Relevant data, that the upload thread needs to complete the upload.
    ///
    /// \author Felix Hommel
    /// \date 2/16/2026
    struct UploadTask
    {
        std::string key;
        LoadResult result;
    };

    static constexpr auto THREAD_POOL_SIZE_SCALAR{ 4 };

    std::unique_ptr<ThreadPool> m_threadPool{ std::make_unique<ThreadPool>(
        static_cast<std::size_t>(std::max(std::thread::hardware_concurrency() / THREAD_POOL_SIZE_SCALAR, 1u))
    ) };
    std::unique_ptr<IResourceLoader> m_loader;
    BlockingQueue<UploadTask> m_uploadQueue;
    std::atomic<int> m_inFlight{ 0 };
    std::condition_variable m_done;
    std::mutex m_doneMutex;

    ResourceCache<Shader> m_shaderCache;
    ResourceCache<Texture2D> m_textureCache;

    void enqueueLoadTask(const ShaderLoadRequest& request);
    void enqueueLoadTask(const TextureLoadRequest& request);

    void processUploadTask(const UploadTask& task);
    void uploadToGPU(const std::string& key, const ShaderSourceData& data);
    void uploadToGPU(const std::string& key, const TextureRawData& data);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_CONTEXT_HPP

