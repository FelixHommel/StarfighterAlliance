#include "screens/ScreenStack.hpp"
#include "core/RenderContext.hpp"
#include "screens/ScreenCommand.hpp"

#include "mocks/MockOverlayScreen.hpp"
#include "mocks/MockScreen.hpp"

#include "gmock/gmock.h"
#include <gtest/gtest.h>

#include <memory>
#include <utility>

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

/// \brief Test that the \ref ScreenStack is initially empty.
///
/// When the \ref ScreenStack is constructed, the default construction should result in an empty stack.
TEST_F(ScreenStackTest, DefaultConstructionMakesEmptyStack)
{
    EXPECT_EQ(0, m_stack->size());
}

/// \brief Test that the \ref ScreenStack can process a push \ref ScreenCommand.
///
/// When a push command was queued and processed the size should increase by one, indicating that a new screen was
/// pushed to the stack.
TEST_F(ScreenStackTest, EnqueuePushCommand)
{
    const auto oldSize{ m_stack->size() };

    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::make_unique<MockScreen>() });
    m_stack->processCommands();

    EXPECT_EQ(oldSize + 1, m_stack->size());
}

/// \brief Test that the \ref ScreenStack can process a pop \ref ScreenCommand
///
/// When a pop command was queued and processed the size should decrease by one, indicating that the screen is not
/// needed anymore.
TEST_F(ScreenStackTest, EnqueuePopCommand)
{
    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::make_unique<MockScreen>() });
    m_stack->processCommands();
    const auto oldSize{ m_stack->size() };

    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Pop });
    m_stack->processCommands();

    EXPECT_EQ(oldSize - 1, m_stack->size());
}

/// \brief Test that the \ref ScreenStack can process a replace \ref ScreenCommand
///
/// When a replace command was queued and processed the size should not change because one screen is poped and a new
/// one is emplaced instantly.
TEST_F(ScreenStackTest, EnqueueReplaceCommand)
{
    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::make_unique<MockScreen>() });
    m_stack->processCommands();
    const auto oldSize{ m_stack->size() };

    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Replace, .screen = std::make_unique<MockScreen>() });
    m_stack->processCommands();

    EXPECT_EQ(oldSize, m_stack->size());
}

/// \brief Test that the \ref ScreenStack calls the onEnter callback function when a new screen is pushed.
///
/// When a new screen is pushed the \ref ScreenStack should call the user specified on enter function.
TEST_F(ScreenStackTest, ScreenStackPushCallsOnEnterFunction)
{
    constexpr auto CALLBACK_VAR_INIT{ 0 };
    auto callbackVar{ CALLBACK_VAR_INIT };

    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::make_unique<MockScreen>([&callbackVar]{ callbackVar++; }) });
    m_stack->processCommands();

    EXPECT_EQ(CALLBACK_VAR_INIT + 1, callbackVar);
}

/// \brief Test that the \ref ScreenStack calls the onExit callback function when a screen is poped from the stack.
///
/// When a screen is poped, the \ref ScreenStack is supposed to call the user specified on exit function
TEST_F(ScreenStackTest, ScreenStackPopCallsOnExitFunction)
{
    constexpr auto CALLBACK_VAR_INIT{ 0 };
    auto callbackVar{ CALLBACK_VAR_INIT };

    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Pop, .screen = std::make_unique<MockScreen>([]{}, [&callbackVar]{ callbackVar--; }) });
    m_stack->processCommands();

    EXPECT_EQ(CALLBACK_VAR_INIT - 1, callbackVar);
}

/// \brief Test that the \ref ScreenStack first calls the onExit function of the old screen that is being replaced and
/// then calls the onEnter function of the screen being pushed.
///
/// When a screen is replaced, the \ref ScreenStack should first use the on exit function of the screen that is being
/// replaced and then should call the on enter function of the new screen that is being pushed.
TEST_F(ScreenStackTest, ScreenStackReplaceCallsOnEnterAndOnExitFunctions)
{
    constexpr auto CALLBACK_VAR_INIT{ 0 };
    auto callbackVar{ CALLBACK_VAR_INIT };

    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Replace, .screen = std::make_unique<MockScreen>([&callbackVar]{ callbackVar++; }, [&callbackVar]{ callbackVar--; }) });
    m_stack->processCommands();

    EXPECT_EQ(CALLBACK_VAR_INIT, callbackVar);
}

/// \brief Test that the \ref ScreenStack correctly renders overlays and regular screens.
///
/// When there are overlays on the top of the stack, the render function should render all overlays and the first
/// non-overlay screen.
TEST_F(ScreenStackTest, ScreenStackRendersAllRelevantScreens)
{
    auto firstScreen{ std::make_unique<MockScreen>() };
    auto secondScreen{ std::make_unique<MockScreen>() };
    auto firstOverlay{ std::make_unique<MockOverlayScreen>() };
    auto secondOverlay{ std::make_unique<MockOverlayScreen>() };

    EXPECT_CALL(*firstScreen.get(), render(::testing::_)).Times(::testing::Exactly(0));
    EXPECT_CALL(*secondScreen.get(), render(::testing::_)).Times(::testing::Exactly(1));
    EXPECT_CALL(*firstOverlay.get(), render(::testing::_)).Times(::testing::Exactly(1));
    EXPECT_CALL(*secondOverlay.get(), render(::testing::_)).Times(::testing::Exactly(1));

    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::move(firstScreen) });
    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::move(secondScreen) });
    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::move(firstOverlay) });
    m_stack->enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::move(secondOverlay) });
    m_stack->processCommands();

    m_stack->render({});
}

} // namespace sfa::testing

