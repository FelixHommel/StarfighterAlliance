#ifndef SFA_SRC_ENGINE_SCREENS_SELECTION_SCREEN_HPP
#define SFA_SRC_ENGINE_SCREENS_SELECTION_SCREEN_HPP

#include "config/SpaceshipType.hpp"
#include "core/RenderContext.hpp"
#include "core/resourceManagement/ResourceContext.hpp"
#include "ecs/ComponentRegistry.hpp"
#include "screens/GameSession.hpp"
#include "screens/IScreen.hpp"
#include "screens/IScreenContext.hpp"
#include "utility/IWindow.hpp"
#include "utility/userInput/InputController.hpp"

#include <array>
#include <functional>
#include <memory>
#include <string>
#include <utility>

namespace sfa
{

/// \brief The \ref SelectionScreen is used to select the desired spaceship
///
/// \author Felix Hommel
/// \date 5/24/2026
class SelectionScreen : public IScreen
{
public:
    using OnEnterFunction = std::function<void(void)>;
    using OnExitFunction = std::function<void(void)>;

    /// \brief Construct a new \ref SelectionScreen
    ///
    /// \param context \ref IScreenContext that is used to help transition between screens.
    /// \param session \ref GameSession to update the session state.
    /// \param viewport \ref Viewport to help size the UI elements appropriately.
    /// \param resources \ref ResourceContext to load spaceship preview images.
    SelectionScreen(
        IScreenContext& context, GameSession& session, const Viewport& viewport, ResourceContext& resources
    );
    ~SelectionScreen() override = default;

    SelectionScreen(const SelectionScreen&) = delete;
    SelectionScreen& operator=(const SelectionScreen&) = delete;
    SelectionScreen(SelectionScreen&&) noexcept = delete;
    SelectionScreen& operator=(SelectionScreen&&) noexcept = delete;

    void onEnter() override;
    void onExit() override;

    void update(float dt, const InputController& controller) override;
    void render(const RenderContext& context) override;

    bool isOverlay() override { return false; }

private:
    struct SpaceshipData
    {
        unsigned int health;
        unsigned int speed;
        unsigned int damage;
        std::string textureName;
    };

    IScreenContext& m_screenContext;
    GameSession& m_session;
    OnEnterFunction m_onEnterFunction{ []() {} };
    OnExitFunction m_onExitFunction{ [] {} };

    ComponentRegistry m_registry;
    std::array<std::unique_ptr<SpaceshipData>, std::to_underlying(SpaceshipType::SPACESHIP_TPYE_MAX)> m_spaceships;
    SpaceshipType m_currentSelection{ SpaceshipType::XWing };

    void createCorePanelsUI(const Viewport& viewport);
    void createSpaceshipPreviewUI(const ResourceContext& resources);
    void createBottomButtonsUI(const Viewport& viewport);

    void loadSpaceshipData(ResourceContext& resources);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_SCREENS_SELECTION_SCREEN_HPP
