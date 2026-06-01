#ifndef SFA_SRC_TEST_MOCKS_MOCK_WINDOW
#define SFA_SRC_TEST_MOCKS_MOCK_WINDOW

#include "utility/IWindow.hpp"
#include "utility/userInput/InputController.hpp"

namespace sfa::testing
{

/// \brief Mock implementation of a \ref IWindow for use while testing
///
/// \author Felix Hommel
/// \date 5/23/2026
class MockWindow final : public IWindow
{
public:
    MockWindow() = default;
    ~MockWindow() override = default;

    MockWindow(const MockWindow&) = delete;
    MockWindow& operator=(const MockWindow&) = delete;
    MockWindow(MockWindow&&) = delete;
    MockWindow& operator=(MockWindow&&) = delete;

    [[nodiscard]] bool shouldClose() const override { return m_shouldClose; }
    [[nodiscard]] Viewport viewport() const noexcept override { return m_viewport; }
    void setShouldClose() override { m_shouldClose = true; }
    void attachInputController(InputController* controller) override {}

    void setViewport(const Viewport& viewport) { m_viewport = viewport; }

private:
    bool m_shouldClose{ false };
    sfa::Viewport m_viewport{ .width = 0, .height = 0 };
};

} // namespace sfa::testing

#endif // !SFA_SRC_TEST_MOCKS_MOCK_WINDOW
