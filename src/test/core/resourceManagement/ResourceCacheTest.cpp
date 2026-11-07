#include "core/resourceManagement/ResourceCache.hpp"
#include "utility/exceptions/ResourceUnavailableException.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

namespace
{

/// \brief Resource that can be used for testing purposes.
///
/// \author Felix Hommel
/// \date 2/16/2026
struct TestResource
{
    int data;
};

constexpr auto TEST_KEY{ "key" };
constexpr auto TEST_DATA{ 42 };

} // namespace

namespace sfa::testing
{

/// \brief Test the features of the \ref ResourceCache.
///
/// \author Felix Hommel
/// \date 2/16/2026
TEST_CASE("ResourceCacheTest", "[unit][core]")
{
    ResourceCache<TestResource> cache{};

    /// \brief Test inserting a new resource into the cache
    ///
    /// This should store the resource in the cache, increasing its size and making it available to be accessed.
    SECTION("Store resource")
    {
        cache.store(::TEST_KEY, std::make_shared<TestResource>(::TEST_DATA));

        CHECK(cache.size() == 1);
        CHECK(cache.contains(::TEST_KEY));
    }

    /// \brief Test inserting a value into a key that was previously inserted.
    ///
    /// When a value is inserted for a key that already has a value associated with it, the old value is overridden by the
    /// new value.
    SECTION("Override resource")
    {
        cache.store(::TEST_KEY, std::make_shared<TestResource>(::TEST_DATA));
        cache.store(::TEST_KEY, std::make_shared<TestResource>(::TEST_DATA + 1));

        CHECK(cache.size() == 1);
        CHECK(cache.contains(::TEST_KEY));
    }

    /// \brief Test getting a resource that was previously inserted into the cache.
    ///
    /// When the resource was stored previously, it should be accessible via the get method.
    SECTION("Get existing resource")
    {
        auto resource{ std::make_shared<TestResource>(::TEST_DATA) };

        cache.store(::TEST_KEY, resource);

        const auto res{ cache.get(::TEST_KEY) };
        CHECK(res->data == ::TEST_DATA);
    }

    /// \brief Test getting a resource that wasn't previously inserted into the cache.
    ///
    /// When the resource wasn't stored prior to trying to get it, the get mthod should throw an exception.
    SECTION("Get non existing resource")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        REQUIRE_THROWS_MATCHES(
            cache.get(::TEST_KEY), ResourceUnavailableException, MessageMatches(ContainsSubstring(""))
        );
    }

    /// \brief Test getting a resource that was previously inserted into the cache.
    ///
    /// When the resource was stored previously, it should be accessible via the get method.
    SECTION("Const-get existing resource")
    {
        ResourceCache<TestResource> cache;
        const auto& constCache{ cache };

        cache.store(TEST_KEY, std::make_shared<TestResource>(::TEST_DATA));

        const auto res{ constCache.get(::TEST_KEY) };
        CHECK(res->data == ::TEST_DATA);
    }

    /// \brief Test getting a resource that wasn't previously inserted into the cache.
    ///
    /// When the resource wasn't stored prior to trying to get it, the get mthod should throw an exception.
    SECTION("Const-get non existing resource")
    {
        using Catch::Matchers::ContainsSubstring;
        using Catch::Matchers::MessageMatches;

        ResourceCache<TestResource> cache;
        const auto& constCache{ cache };

        REQUIRE_THROWS_MATCHES(
            constCache.get(::TEST_KEY), ResourceUnavailableException, MessageMatches(ContainsSubstring(""))
        );
    }

    /// \brief Test clearing the cache.
    ///
    /// When the cache is cleared, there shouldn't be any values left in it.
    SECTION("Clear cache")
    {
        cache.store(TEST_KEY, std::make_shared<TestResource>(::TEST_DATA));

        cache.clear();

        CHECK(cache.empty());
    }
}

} // namespace sfa::testing

