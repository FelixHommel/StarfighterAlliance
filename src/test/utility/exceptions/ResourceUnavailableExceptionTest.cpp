#include "utility/exceptions/ResourceUnavailableException.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

constexpr auto TEST_MESSAGE{ "TestMessage" };
constexpr auto TEST_KEY{ "Key" };
constexpr auto throwingStatement = [] { throw sfa::ResourceUnavailableException{ TEST_MESSAGE, TEST_KEY }; };

} // namespace

namespace sfa::testing
{

/// \brief Test the most basic way to create an exception.
///
/// In this case the source location should be the exact line where, the exception was thrown, in the same function.
TEST(ResourceUnavailableExceptionTest, DefaultConstruction)
{
    ResourceUnavailableException e{ ::TEST_MESSAGE, ::TEST_KEY };

    EXPECT_STREQ(e.location().function_name(), std::source_location::current().function_name());
}

/// \brief Test that the exception contains the user message when it is thrown.
///
/// When the exception is thrown, the message should contain the message the user specified
TEST(ResourceUnavailableExceptionTest, ExceptionMessgae)
{
    EXPECT_THAT(::throwingStatement, ::testing::ThrowsMessage<ResourceUnavailableException>(::testing::HasSubstr(::TEST_MESSAGE)));
}

} // namespace sfa::testing

