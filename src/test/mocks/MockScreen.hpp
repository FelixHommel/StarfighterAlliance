#ifndef SFA_SRC_TEST_MOCKS_MOCK_SCREEN_HPP
#define SFA_SRC_TEST_MOCKS_MOCK_SCREEN_HPP

#include "screens/IScreen.hpp"

namespace sfa::testing
{

class MockScreen : public IScreen
{
public:
    using OnEnterFunction = std::function<void(void)>;
    using OnExitFunction = std::function<void(void)>;

    MockScreen(OnEnterFunction onEnter, OnExitFunction onExit);
    ~MockScreen() override = default;

    MockScreen(const MockScreen&) = delete;
    MockScreen& operator=(const MockScreen&) = delete;
    MockScreen(MockScreen&&) noexcept = default;
    MockScreen& operator=(MockScreen&&) noexcept = default;

    void onEnter() override{ if(m_onEnterFunction) m_onEnterFunction();}
    void onExit() override{ if(m_onExitFunction) m_onExitFunction(); }

    void handleInput(const InputController& controller) override {}
    void update(float dt) override {}
    void render(const RenderContext& context) override {}

    bool isOverlay() override { return false; }

private:
    OnEnterFunction m_onEnterFunction;
    OnExitFunction m_onExitFunction;
};

} // namespace sfa::testing

#endif // !SFA_SRC_TEST_MOCKS_MOCK_SCREEN_HPP

