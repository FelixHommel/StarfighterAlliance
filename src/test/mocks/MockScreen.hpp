#ifndef SFA_SRC_TEST_MOCKS_MOCK_SCREEN_HPP
#define SFA_SRC_TEST_MOCKS_MOCK_SCREEN_HPP

#include "screens/IScreen.hpp"

#include "gmock/gmock.h"

#include <utility>

namespace sfa::testing
{

/// \brief Mock implementation of a Screen.
///
/// \author Felix Hommel
/// \date 3/1/2026
class MockScreen : public IScreen
{
public:
    using OnEnterFunction = std::function<void(void)>;
    using OnExitFunction = std::function<void(void)>;

    MockScreen(OnEnterFunction onEnter = []{}, OnExitFunction onExit = []{}) : m_onEnterFunction(std::move(onEnter)), m_onExitFunction(std::move(onExit)) {}
    ~MockScreen() override = default;

    MockScreen(const MockScreen&) = delete;
    MockScreen& operator=(const MockScreen&) = delete;
    MockScreen(MockScreen&&) noexcept = delete;
    MockScreen& operator=(MockScreen&&) noexcept = delete;

    void onEnter() override{ if(m_onEnterFunction) m_onEnterFunction();}
    void onExit() override{ if(m_onExitFunction) m_onExitFunction(); }

    MOCK_METHOD(void, handleInput, (const InputController& controller), (override));
    MOCK_METHOD(void, update, (float dt), (override));
    MOCK_METHOD(void, render, (const RenderContext& context), (override));

    bool isOverlay() override { return false; }

private:
    OnEnterFunction m_onEnterFunction;
    OnExitFunction m_onExitFunction;
};

} // namespace sfa::testing

#endif // !SFA_SRC_TEST_MOCKS_MOCK_SCREEN_HPP

