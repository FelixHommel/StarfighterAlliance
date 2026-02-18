#ifndef SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_CACHE_HPP
#define SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_CACHE_HPP

#include "spdlog/spdlog.h"
#include "fmt/format.h"

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace sfa
{

/// \brief A resource cache is a container that stores a particular type of resource.
///
/// Type-safe storage in a map, so that the resource can be accessed by a name. This requires that resources are move
/// constructable.
///
/// \author Felix Hommel
/// \date 1/21/2026
template<typename T>
class ResourceCache
{
public:
    ResourceCache() = default;
    ~ResourceCache() = default;

    ResourceCache(const ResourceCache&) = delete;
    ResourceCache& operator=(const ResourceCache&) = delete;
    ResourceCache(ResourceCache&&) noexcept = delete;
    ResourceCache& operator=(ResourceCache&&) noexcept = delete;

    /// \brief Store a new resource.
    ///
    /// The method will overwrite an existing resource if the key already is in use.
    ///
    /// \param key name by which the resource will be accessible
    /// \param resource the resource that is stored
    void store(std::string key, std::shared_ptr<T> resource)
    {
        if(m_resources.contains(key))
            spdlog::warn("Overwritten a resource with key: {}", key);

        m_resources.insert_or_assign(std::move(key), std::move(resource));
    }

    /// \brief Access a resource.
    ///
    /// \param key which resource to access
    ///
    /// \returns shared_ptr to the resource
    ///
    /// \throws \ref std::runtime_error if there is no resource stored under the name \p key
    [[nodiscard]] std::shared_ptr<T> get(const std::string& key)
    {
        const auto it{ m_resources.find(key) };
        if(it == m_resources.end())
        {
            // TODO: replace with custom exception
            throw std::runtime_error(fmt::format("No resource not found at key '{}'", key));
        }

        return it->second;
    }

    /// \brief Access a resource.
    ///
    /// \param key which resource to access
    ///
    /// \returns shared_ptr to the resource
    ///
    /// \throws \ref std::runtime_error if there is no resource stored under the name \p key
    [[nodiscard]] std::shared_ptr<T> get(const std::string& key) const
    {
        const auto it{ m_resources.find(key) };
        if(it == m_resources.end())
        {
            // TODO: replace with custom exception
            throw std::runtime_error(fmt::format("No resource not found at key '{}'", key));
        }

        return it->second;
    }

    [[nodiscard]] bool contains(const std::string& key) const { return m_resources.contains(key); }
    [[nodiscard]] bool empty() const noexcept { return m_resources.empty(); }
    [[nodiscard]] std::size_t size() const noexcept { return m_resources.size(); }
    void clear() noexcept { m_resources.clear(); }

private:
    std::unordered_map<std::string, std::shared_ptr<T>> m_resources;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_CORE_RESOURCE_MANAGEMENT_RESOURCE_CACHE_HPP

