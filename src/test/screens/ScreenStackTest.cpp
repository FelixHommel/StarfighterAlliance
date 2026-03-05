#include "screens/ScreenStack.hpp"
#include "mocks/MockScreen.hpp"

#include <gtest/gtest.h>

#include <memory>

namespace sfa::testing
{

/// \brief Test the features and functions of the \ref ScreenStack.
///
/// \author Felix Hommel
/// \date 3/4/2026
class ScreenStackTest : public ::testing::Test
{
public:
    ScreenStackTest() = default;
    ~ScreenStackTest() override = default;

    ScreenStackTest(const ScreenStackTest&) = delete;
    ScreenStackTest& operator=(const ScreenStackTest&) = delete;
    ScreenStackTest(ScreenStackTest&&) noexcept = delete;
    ScreenStackTest& operator=(ScreenStackTest&&) noexcept = delete;

protected:
    std::unique_ptr<ScreenStack> m_stack;
};

TEST_F(ScreenStackTest, EnqueuePushCommand)
{
    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::make_unique<MockScreen>([]{}, []{}) });

    m_stack->processCommands();
}

TEST_F(ScreenStackTest, EnqueuePopCommand)
{
}

TEST_F(ScreenStackTest, EnqueueReplaceCommand)
{
}

} // namespace sfa::testing

