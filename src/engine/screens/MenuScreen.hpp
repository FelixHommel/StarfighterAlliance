#ifndef SFA_SRC_ENGINE_SCREENS_MENU_SCREEN_HPP
#define SFA_SRC_ENGINE_SCREENS_MENU_SCREEN_HPP

#include "core/RenderContext.hpp"
#include "core/resourceManagement/ResourceContext.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "screens/GameSession.hpp"
#include "screens/IScreen.hpp"
#include "screens/IScreenContext.hpp"
#include "utility/IWindow.hpp"
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

    /// \brief Construct a new \ref MenuScreen
    ///
    /// \param context \ref IScreenContext that is used to help transition between screens
    /// \param window \ref IWindow needed for window size and quitting
    /// \param session \ref GameSession to update the session state
    MenuScreen(IScreenContext& context, IWindow& window, GameSession& session);
    ~MenuScreen() override = default;

    MenuScreen(const MenuScreen&) = delete;
    MenuScreen& operator=(const MenuScreen&) = delete;
    MenuScreen(MenuScreen&&) noexcept = delete;
    MenuScreen& operator=(MenuScreen&&) noexcept = delete;

    void onEnter() override;
    void onExit() override;

    void update(float dt, const InputController& controller) override;
    void render(const RenderContext& context) override;

    bool isOverlay() override { return false; }

private:
    IScreenContext& m_screenContext;
    IWindow& m_window;
    GameSession& m_session;
    OnEnterFunction m_onEnterFunction;
    OnExitFunction m_onExitFunction{ [] {} };

    ComponentRegistry m_registry;
    ResourceContext resources; // FIXME: This should not be owned here

    void createRootPanelUI();
    void createPlayButtonUI();
    void createQuitButtonUI();

    static void makeColorRequest(GameSession& session);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_SCREENS_MENU_SCREEN_HPP

