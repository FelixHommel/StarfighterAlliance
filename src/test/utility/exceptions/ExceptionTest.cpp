#include "utility/exceptions/Exception.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <source_location>

namespace
{

constexpr auto TEST_MESSAGE{ "TestMessage" };
constexpr auto throwingStatement = [] {
    throw sfa::Exception{ TEST_MESSAGE };
};

} // namespace

namespace sfa::testing
{

TEST_CASE("ExceptionTest", "[unit][utility]")
{
    using Catch::Matchers::ContainsSubstring;
    using Catch::Matchers::MessageMatches;

    /// \brief Test the most basic way to create an exception.
    ///
    /// In this case the source location should be the exact line where, the exception was thrown, in the same function.
    SECTION("Default construction")
    {
        Exception e{ ::TEST_MESSAGE };

        CHECK_THAT(e.location().function_name(), ContainsSubstring(std::source_location::current().function_name()));
    }

    /// \brief Test that the exception contains the user message when it is thrown.
    ///
    /// When the exception is thrown, the message should contain the message the user specified
    SECTION("Exception message")
    {
        CHECK_THROWS_MATCHES(::throwingStatement, Exception, MessageMatches(ContainsSubstring(::TEST_MESSAGE)));
    }
}

} // namespace sfa::testing

