#include "core/resourceManagement/ResourceCache.hpp"
#include "utility/exceptions/ResourceUnavailableException.hpp"

#include <gtest/gtest.h>

#include <memory>

namespace sfa::testing
{

/// \brief Resource that can be used for testing purposes.
///
/// \author Felix Hommel
/// \date 2/16/2026
struct TestResource
{
    int data;
};

/// \brief Test the features of the \ref ResourceCache.
///
/// \author Felix Hommel
/// \date 2/16/2026
class ResourceCacheTest : public ::testing::Test
{
public:
    ResourceCacheTest() = default;
    ~ResourceCacheTest() override = default;

    ResourceCacheTest(const ResourceCacheTest&) = delete;
    ResourceCacheTest& operator=(const ResourceCacheTest&) = delete;
    ResourceCacheTest(ResourceCacheTest&&) = delete;
    ResourceCacheTest& operator=(ResourceCacheTest&&) = delete;

protected:
    static constexpr auto K{ "key" };
    static constexpr auto DATA{ 42 };

    std::unique_ptr<ResourceCache<TestResource>> m_cache{ std::make_unique<ResourceCache<TestResource>>() };
};

/// \brief Test inserting a new resource into the cache
///
/// This should store the resource in the cache, increasing its size and making it available to be accessed.
TEST_F(ResourceCacheTest, StoreResource)
{
    m_cache->store(K, std::make_shared<TestResource>(DATA));

    EXPECT_EQ(1, m_cache->size());
    EXPECT_TRUE(m_cache->contains(K));
}

/// \brief Test inserting a value into a key that was previously inserted.
///
/// When a value is inserted for a key that already has a value associated with it, the old value is overridden by the
/// new value.
TEST_F(ResourceCacheTest, OverrideResource)
{
    m_cache->store(K, std::make_shared<TestResource>(DATA));
    m_cache->store(K, std::make_shared<TestResource>(DATA + 1));

    EXPECT_EQ(1, m_cache->size());
    EXPECT_TRUE(m_cache->contains(K));
}

/// \brief Test getting a resource that was previously inserted into the cache.
///
/// When the resource was stored previously, it should be accessible via the get method.
TEST_F(ResourceCacheTest, GetExistingResource)
{
    auto resource{ std::make_shared<TestResource>(DATA) };

    m_cache->store(K, resource);

    const auto res{ m_cache->get(K) };
    EXPECT_EQ(DATA, res->data);
}

/// \brief Test getting a resource that wasn't previously inserted into the cache.
///
/// When the resource wasn't stored prior to trying to get it, the get mthod should throw an exception.
TEST_F(ResourceCacheTest, GetNonExistingResource)
{
    EXPECT_THROW({ const auto x{ m_cache->get(K) }; }, ResourceUnavailableException);
}

/// \brief Test getting a resource that was previously inserted into the cache.
///
/// When the resource was stored previously, it should be accessible via the get method.
TEST_F(ResourceCacheTest, ConstGetExistingResource)
{
    ResourceCache<TestResource> cache;
    const auto& constCache{ cache };

    cache.store(K, std::make_shared<TestResource>(DATA));

    const auto res{ constCache.get(K) };
    EXPECT_EQ(DATA, res->data);
}

/// \brief Test getting a resource that wasn't previously inserted into the cache.
///
/// When the resource wasn't stored prior to trying to get it, the get mthod should throw an exception.
TEST_F(ResourceCacheTest, ConstGetNonExistingResource)
{
    ResourceCache<TestResource> cache;
    const auto& constCache{ cache };

    EXPECT_THROW({ const auto x{ constCache.get(K) }; }, ResourceUnavailableException);
}

/// \brief Test clearing the cache.
///
/// When the cache is cleared, there shouldn't be any values left in it.
TEST_F(ResourceCacheTest, ClearCache)
{
    m_cache->store(K, std::make_shared<TestResource>(DATA));

    m_cache->clear();

    EXPECT_TRUE(m_cache->empty());
}

} // namespace sfa::testing

