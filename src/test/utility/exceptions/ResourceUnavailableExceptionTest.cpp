#include "utility/exceptions/ResourceUnavailableException.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

namespace
{

constexpr auto TEST_MESSAGE{ "TestMessage" };
constexpr auto TEST_KEY{ "Key" };
constexpr auto throwingStatement = [] {
    throw sfa::ResourceUnavailableException{ TEST_MESSAGE, TEST_KEY };
};

} // namespace

namespace sfa::testing
{

TEST_CASE("ResourceUnavailableExceptionTest", "[unit][utility]")
{
    using Catch::Matchers::ContainsSubstring;
    using Catch::Matchers::MessageMatches;

    /// \brief Test the most basic way to create an exception.
    ///
    /// In this case the source location should be the exact line where, the exception was thrown, in the same function.
    SECTION("Default construction")
    {
        ResourceUnavailableException e{ ::TEST_MESSAGE, ::TEST_KEY };

        CHECK_THAT(e.location().function_name(), ContainsSubstring(std::source_location::current().function_name()));
    }

    /// \brief Test that the exception contains the user message when it is thrown.
    ///
    /// When the exception is thrown, the message should contain the message the user specified
    SECTION("Exception messgae")
    {
        CHECK_THROWS_MATCHES(
            ::throwingStatement, ResourceUnavailableException, MessageMatches(ContainsSubstring(::TEST_MESSAGE))
        );
    }
}

} // namespace sfa::testing

