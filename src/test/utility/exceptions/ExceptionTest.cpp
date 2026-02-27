#include "utility/exceptions/Exception.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

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

/// \brief Test the most basic way to create an exception.
///
/// In this case the source location should be the exact line where, the exception was thrown, in the same function.
TEST(ExceptionTest, DefaultConstruction)
{
    Exception e{ ::TEST_MESSAGE };

    EXPECT_STREQ(e.location().function_name(), std::source_location::current().function_name());
}

/// \brief Test that the exception contains the user message when it is thrown.
///
/// When the exception is thrown, the message should contain the message the user specified
TEST(ExceptionTest, ExceptionMessgae)
{
    EXPECT_THAT(::throwingStatement, ::testing::ThrowsMessage<Exception>(::testing::HasSubstr(::TEST_MESSAGE)));
}

} // namespace sfa::testing

