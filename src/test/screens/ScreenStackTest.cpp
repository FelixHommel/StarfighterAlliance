#include "screens/ScreenStack.hpp"
#include "screens/ScreenCommand.hpp"
#include "testUtility//mocks/MockScreen.hpp"
#include "testUtility/mocks/MockOverlayScreen.hpp"
#include "testUtility/mocks/MockWindow.hpp"
#include "utility/userInput/InputController.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>
#include <trompeloeil/mock.hpp>

#include <memory>
#include <utility>

namespace sfa::testing
{

/// \brief Test the features and functions of the \ref ScreenStack.
///
/// \author Felix Hommel
/// \date 3/4/2026
TEST_CASE("ScreenStackTest", "[unit][screens]")
{
    ScreenStack m_stack{};

    /// \brief Test that the \ref ScreenStack is initially empty.
    ///
    /// When the \ref ScreenStack is constructed, the default construction should result in an empty stack.
    SECTION("Default construction makes empty stack")
    {
        CHECK(m_stack.size() == 0);
    }

    SECTION("Parameter constructor adds initial screen")
    {
        ScreenStack stack{ std::make_unique<MockScreen>() };

        CHECK(stack.size() == 1);
    }

    /// \brief Test that the \ref ScreenStack can process a push \ref ScreenCommand.
    ///
    /// When a push command was queued and processed the size should increase by one, indicating that a new screen was
    /// pushed to the stack.
    SECTION("Enqueue push command")
    {
        const auto oldSize{ m_stack.size() };

        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::make_unique<MockScreen>() });
        m_stack.processCommands();

        CHECK(m_stack.size() == oldSize + 1);
    }

    /// \brief Test that the \ref ScreenStack can process a pop \ref ScreenCommand
    ///
    /// When a pop command was queued and processed the size should decrease by one, indicating that the screen is not
    /// needed anymore.
    SECTION("Enqueue pop command")
    {
        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::make_unique<MockScreen>() });
        m_stack.processCommands();
        const auto oldSize{ m_stack.size() };

        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Pop });
        m_stack.processCommands();

        CHECK(m_stack.size() == oldSize - 1);
    }

    /// \brief Test that the \ref ScreenStack can process a replace \ref ScreenCommand
    ///
    /// When a replace command was queued and processed the size should not change because one screen is poped and a new
    /// one is emplaced instantly.
    SECTION("Enqueue replace command")
    {
        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::make_unique<MockScreen>() });
        m_stack.processCommands();
        const auto oldSize{ m_stack.size() };

        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Replace, .screen = std::make_unique<MockScreen>() });
        m_stack.processCommands();

        CHECK(m_stack.size() == oldSize);
    }

    /// \brief Test that the \ref ScreenStack calls the onEnter callback function when a new screen is pushed.
    ///
    /// When a new screen is pushed the \ref ScreenStack should call the user specified on enter function.
    SECTION("Screen stack push calls on enter function")
    {
        constexpr auto CALLBACK_VAR_INIT{ 0 };
        auto callbackVar{ CALLBACK_VAR_INIT };

        m_stack.enqueueCommand(
            {
                .type = ScreenCommand::Type::Push,
                .screen = std::make_unique<MockScreen>([&callbackVar] { callbackVar++; }),
            }
        );
        m_stack.processCommands();

        CHECK(callbackVar == CALLBACK_VAR_INIT + 1);
    }

    /// \brief Test that the \ref ScreenStack calls the onExit callback function when a screen is poped from the stack.
    ///
    /// When a screen is poped, the \ref ScreenStack is supposed to call the user specified on exit function
    SECTION("Screen stack pop calls on exit function")
    {
        constexpr auto CALLBACK_VAR_INIT{ 0 };
        auto callbackVar{ CALLBACK_VAR_INIT };

        m_stack.enqueueCommand(
            {
                .type = ScreenCommand::Type::Push,
                .screen = std::make_unique<MockScreen>([] {}, [&callbackVar] { callbackVar--; }),
            }
        );
        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Pop, .screen = nullptr });
        m_stack.processCommands();

        CHECK(callbackVar == CALLBACK_VAR_INIT - 1);
    }

    /// \brief Test that the \ref ScreenStack first calls the onExit function of the old screen that is being replaced and
    /// then calls the onEnter function of the screen being pushed.
    ///
    /// When a screen is replaced, the \ref ScreenStack should first use the on exit function of the screen that is being
    /// replaced and then should call the on enter function of the new screen that is being pushed.
    SECTION("Screen stack replace calls on enter and on exit functions")
    {
        constexpr auto CALLBACK_VAR_INIT_FIRST_SCREEN{ 1 };
        auto callbackVarFirstScreen{ CALLBACK_VAR_INIT_FIRST_SCREEN };
        constexpr auto CALLBACK_VAR_INIT_SECOND_SCREEN{ 1 };
        auto callbackVarSecondScreen{ CALLBACK_VAR_INIT_SECOND_SCREEN };

        m_stack.enqueueCommand(
            {
                .type = ScreenCommand::Type::Push,
                .screen = std::make_unique<MockScreen>([] {}, [&callbackVarFirstScreen] { callbackVarFirstScreen--; }),
            }
        );
        m_stack.enqueueCommand(
            {
                .type = ScreenCommand::Type::Replace,
                .screen = std::make_unique<MockScreen>([&callbackVarSecondScreen] { callbackVarSecondScreen++; }),
            }
        );
        m_stack.processCommands();

        CHECK(callbackVarFirstScreen == CALLBACK_VAR_INIT_FIRST_SCREEN - 1);
        CHECK(callbackVarSecondScreen == CALLBACK_VAR_INIT_SECOND_SCREEN + 1);
    }

    /// \brief Test that the \ref ScreenStack renders one singular screen correctly
    ///
    /// When there is only one screen in the stack, it should be rendered
    SECTION("Screen stack renders one screen")
    {
        MockWindow window{};

        auto screen{ std::make_unique<MockScreen>() };

        REQUIRE_CALL(*screen.get(), render(trompeloeil::_)).TIMES(1);

        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::move(screen) });
        m_stack.processCommands();

        m_stack.render({ .spriteRenderer = nullptr, .window = window });
    }

    /// \brief Test that the \ref ScreenStack correctly renders overlays and regular screens.
    ///
    /// When there are overlays on the top of the stack, the render function should render all overlays and the first
    /// non-overlay screen.
    SECTION("Screen stack renders all relevant screens")
    {
        MockWindow window{};

        auto firstScreen{ std::make_unique<MockScreen>() };
        auto secondScreen{ std::make_unique<MockScreen>() };
        auto firstOverlay{ std::make_unique<MockOverlayScreen>() };
        auto secondOverlay{ std::make_unique<MockOverlayScreen>() };

        REQUIRE_CALL(*firstScreen.get(), render(trompeloeil::_)).TIMES(0);
        REQUIRE_CALL(*secondScreen.get(), render(trompeloeil::_)).TIMES(1);
        REQUIRE_CALL(*firstOverlay.get(), render(trompeloeil::_)).TIMES(1);
        REQUIRE_CALL(*secondOverlay.get(), render(trompeloeil::_)).TIMES(1);

        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::move(firstScreen) });
        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::move(secondScreen) });
        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::move(firstOverlay) });
        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::move(secondOverlay) });
        m_stack.processCommands();

        m_stack.render({ .spriteRenderer = nullptr, .window = window });
    }

    /// \brief Test that the \ref ScreenStack only updates the top most screen.
    ///
    /// When there are multiple screens in the stack, the update only affects the top most screen even if that is an overlay.
    SECTION("Screen stack updates top most screen")
    {
        constexpr auto TEST_DT{ 0.01f };

        InputController controller{};

        auto screen{ std::make_unique<MockScreen>() };
        auto overlay{ std::make_unique<MockOverlayScreen>() };

        REQUIRE_CALL(*screen.get(), update(trompeloeil::_, trompeloeil::_)).TIMES(0);
        REQUIRE_CALL(*overlay.get(), update(trompeloeil::_, trompeloeil::_)).TIMES(1);

        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::move(screen) });
        m_stack.enqueueCommand({ .type = ScreenCommand::Type::Push, .screen = std::move(overlay) });
        m_stack.processCommands();

        m_stack.update(TEST_DT, controller);
    }
}

} // namespace sfa::testing

