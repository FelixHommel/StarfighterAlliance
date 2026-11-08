#ifndef SFA_SRC_TEST_MOCKS_MOCK_OVERLAY_SCREEN_HPP
#define SFA_SRC_TEST_MOCKS_MOCK_OVERLAY_SCREEN_HPP

#include "screens/IScreen.hpp"

#include <trompeloeil/mock.hpp>

#include <functional>
#include <utility>

namespace sfa::testing
{

/// \brief Mock implementation of a Screen.
///
/// \author Felix Hommel
/// \date 3/1/2026
class MockOverlayScreen : public trompeloeil::mock_interface<IScreen>
{
public:
    using OnEnterFunction = std::function<void(void)>;
    using OnExitFunction = std::function<void(void)>;

    MockOverlayScreen(
        OnEnterFunction onEnter = [] {}, OnExitFunction onExit = [] {}
    )
        : m_onEnterFunction(std::move(onEnter)), m_onExitFunction(std::move(onExit))
    {}
    ~MockOverlayScreen() override = default;

    MockOverlayScreen(const MockOverlayScreen&) = delete;
    MockOverlayScreen& operator=(const MockOverlayScreen&) = delete;
    MockOverlayScreen(MockOverlayScreen&&) noexcept = delete;
    MockOverlayScreen& operator=(MockOverlayScreen&&) noexcept = delete;

    void onEnter() override
    {
        if(m_onEnterFunction)
            m_onEnterFunction();
    }
    void onExit() override
    {
        if(m_onExitFunction)
            m_onExitFunction();
    }

    IMPLEMENT_MOCK2(update);
    IMPLEMENT_MOCK1(render);

    bool isOverlay() override { return true; }

private:
    OnEnterFunction m_onEnterFunction;
    OnExitFunction m_onExitFunction;
};

} // namespace sfa::testing

#endif // !SFA_SRC_TEST_MOCKS_MOCK_OVERLAY_SCREEN_HPP

