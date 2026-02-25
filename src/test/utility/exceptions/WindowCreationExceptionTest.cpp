#include "utility/exceptions/WindowCreationException.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

constexpr auto TEST_MESSAGE{ "TestMessage" };
constexpr auto throwingStatement = [] { throw sfa::WindowCreationException{ TEST_MESSAGE }; };

} // namespace

namespace sfa::testing
{

/// \brief Test the most basic way to create an exception.
///
/// In this case the source location should be the exact line where, the exception was thrown, in the same function.
TEST(WindowCreationExceptionTest, DefaultConstruction)
{
    WindowCreationException e{ ::TEST_MESSAGE };

    EXPECT_STREQ(e.location().function_name(), std::source_location::current().function_name());
}

/// \brief Test that the exception contains the user message when it is thrown.
///
/// When the exception is thrown, the message should contain the message the user specified
TEST(WindowCreationExceptionTest, ExceptionMessgae)
{
    EXPECT_THAT(::throwingStatement, ::testing::ThrowsMessage<WindowCreationException>(::testing::HasSubstr(::TEST_MESSAGE)));
}

} // namespace sfa::testing

