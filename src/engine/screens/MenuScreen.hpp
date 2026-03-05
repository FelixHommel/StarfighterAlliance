#ifndef SFA_SRC_ENGINE_SCREENS_MENU_SCREEN_HPP
#define SFA_SRC_ENGINE_SCREENS_MENU_SCREEN_HPP

#include "core/RenderContext.hpp"
#include "screens/IScreen.hpp"
#include "utility/userInput/InputController.hpp"

#include <functional>

namespace sfa
{

/// \brief The \ref MenuScreen is the state from which a player can start the game.
///
/// \author Felix Hommel
/// \date 3/3/2026
class MenuScreen : public IScreen
{
public:
    using OnEnterFunction = std::function<void(void)>;
    using OnExitFunction = std::function<void(void)>;

    MenuScreen(OnEnterFunction onEnter, OnExitFunction onExit);
    ~MenuScreen() override = default;

    MenuScreen(const MenuScreen&) = delete;
    MenuScreen& operator=(const MenuScreen&) = delete;
    MenuScreen(MenuScreen&&) noexcept = default;
    MenuScreen& operator=(MenuScreen&&) noexcept = default;

    void onEnter() override;
    void onExit() override;

    void handleInput(const InputController& controller) override;
    void update(float dt) override;
    void render(const RenderContext& context) override;

    bool isOverlay() override { return false; }

private:
    OnEnterFunction m_onEnterFunction;
    OnExitFunction m_onExitFunction;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_SCREENS_MENU_SCREEN_HPP

