#include "MenuScreen.hpp"

#include "concurrency/AsyncValue.hpp"
#include "config/SpaceshipColor.hpp"
#include "core/RenderContext.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/components/SpriteComponent.hpp"
#include "ecs/components/TextComponent.hpp"
#include "ecs/components/UIButtonComponent.hpp"
#include "ecs/components/UIHierarchyComponent.hpp"
#include "ecs/components/UILayoutComponent.hpp"
#include "ecs/components/UILayoutElementComponent.hpp"
#include "ecs/components/UITransformComponent.hpp"
#include "ecs/systems/ButtonSystem.hpp"
#include "ecs/systems/LayoutSystem.hpp"
#include "ecs/systems/UITransformSystem.hpp"
#include "screens/GameSession.hpp"
#include "screens/IScreenContext.hpp"
#include "screens/ScreenCommand.hpp"
#include "screens/SelectionScreen.hpp"
#include "utility/IWindow.hpp"
#include "utility/userInput/InputController.hpp"
#include "utility/userInput/InputEvent.hpp"

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <memory>

namespace
{

constexpr sfa::EntityID root{ 0 };
constexpr sfa::EntityID quitButton{ 1 };
constexpr sfa::EntityID playButton{ 2 };

constexpr auto LAYOUT_SPACING{ 30.f };
constexpr auto LAYOUT_PADDING{ glm::vec2(250.f, 220.f) };

constexpr auto BUTTON_SIZE{ glm::vec2(240.f, 80.f) };
constexpr auto BUTTON_STANDARD_COLOR{ glm::vec3(0.f, 0.f, 1.f) };
constexpr auto BUTTON_PRESS_COOLDOWN{ 0.2f };

constexpr auto PLAY_BUTTON_COLOR{ glm::vec3(0.f, 1.f, 0.f) };
constexpr auto QUIT_BUTTON_COLOR{ glm::vec3(1.f, 0.f, 0.f) };

} // namespace

namespace sfa
{

MenuScreen::MenuScreen(IScreenContext& context, IWindow& window, GameSession& session)
    : m_screenContext(context)
    , m_window(window)
    , m_session(session)
    , m_onEnterFunction([&session = m_session]() { MenuScreen::makeColorRequest(session); })
{
    createRootPanelUI();
    createPlayButtonUI();
    createQuitButtonUI();

    // TODO: Send color request to the color server and save future in \p session
}

void MenuScreen::onEnter()
{
    if(m_onEnterFunction)
        m_onEnterFunction();
}

void MenuScreen::onExit()
{
    if(m_onExitFunction)
        m_onExitFunction();
}

void MenuScreen::update(float dt, const InputController& controller)
{
    LayoutSystem::update(m_registry);
    UITransformSystem::update(m_registry);
    ButtonSystem::update(
        m_registry, dt, controller.mousePosition(), controller.isMousePressed(MouseButton::Left) == InputAction::Press
    );
}

void MenuScreen::render(const RenderContext& context)
{
    context.uiRenderer.render(m_registry, context.window.viewport());
}

/// \brief Create the root UI panel
void MenuScreen::createRootPanelUI()
{
    m_registry.addComponent<UITransformComponent>(
        ::root,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = static_cast<glm::vec2>(m_window.viewport()),
        }
    );
    m_registry.addComponent<UIHierarchyComponent>(
        ::root,
        {
            .parent = NULL_ENTITY,
            .children = { ::playButton, ::quitButton },
    }
    );
    m_registry.addComponent<UILayoutComponent>(
        ::root, { .type = UILayoutComponent::Type::Vertical, .spacing = ::LAYOUT_SPACING, .padding = ::LAYOUT_PADDING }
    );
}

/// \brief Create the play button to start the game
void MenuScreen::createPlayButtonUI()
{
    m_registry.addComponent<UITransformComponent>(
        ::playButton, { .localPosition = glm::vec2(0.f), .worldPosition = glm::vec2(0.f), .size = ::BUTTON_SIZE }
    );
    m_registry.addComponent<UIHierarchyComponent>(::playButton, { .parent = ::root, .children = {} });
    m_registry.addComponent<UILayoutElementComponent>(
        ::playButton, { .preferredSize = ::BUTTON_SIZE, .flexGrow = 0.f }
    );
    m_registry.addComponent<SpriteComponent>(
        ::playButton, { .texture = nullptr, .size = ::BUTTON_SIZE, .color = ::PLAY_BUTTON_COLOR, .renderLayer = 0 }
    );
    m_registry.addComponent<TextComponent>(
        ::playButton,
        {
            .content = "PLAY",
            .offset = glm::vec2(0.f),
            .scale = 1.f,
            .color = glm::vec3(1.f),
            .centerInTransform = true,
            .renderLayer = 1,
        }
    );
    m_registry.addComponent<UIButtonComponent>(
        ::playButton,
        {
            .standardColor = ::BUTTON_STANDARD_COLOR,
            .onClick =
                [&ctx = m_screenContext, &window = m_window, &session = m_session, &resources = resources] {
                    spdlog::info("Play pressed");
                    ctx.enqueueCommand(
                        {
                            .type = ScreenCommand::Type::Push,
                            .screen = std::make_unique<SelectionScreen>(ctx, session, window.viewport(), resources),
                        }
                    );
                },
            .pressCooldownMax = ::BUTTON_PRESS_COOLDOWN,
        }
    );
}

/// \brief Create the quit button to close the game
void MenuScreen::createQuitButtonUI()
{
    m_registry.addComponent<UITransformComponent>(
        ::quitButton, { .localPosition = glm::vec2(0.f), .worldPosition = glm::vec2(0.f), .size = ::BUTTON_SIZE }
    );
    m_registry.addComponent<UIHierarchyComponent>(::quitButton, { .parent = ::root, .children = {} });
    m_registry.addComponent<UILayoutElementComponent>(
        ::quitButton, { .preferredSize = ::BUTTON_SIZE, .flexGrow = 0.f }
    );
    m_registry.addComponent<SpriteComponent>(
        ::quitButton,
        {
            .texture = nullptr,
            .size = ::BUTTON_SIZE,
            .color = ::QUIT_BUTTON_COLOR,
            .renderLayer = 0,
        }
    );
    m_registry.addComponent<TextComponent>(
        ::quitButton,
        {
            .content = "QUIT",
            .offset = glm::vec2(0.f),
            .scale = 1.f,
            .color = glm::vec3(1.f),
            .centerInTransform = true,
            .renderLayer = 1,
        }
    );
    m_registry.addComponent<UIButtonComponent>(
        ::quitButton,
        {
            .standardColor = ::BUTTON_STANDARD_COLOR,
            .onClick =
                [&window = m_window] {
                    spdlog::info("QUIT pressed");
                    window.setShouldClose();
                },
            .pressCooldownMax = ::BUTTON_PRESS_COOLDOWN,
        }
    );
}

void MenuScreen::makeColorRequest(GameSession& session)
{
    // TODO: Implement actual request
    if(false) // NOLINT
        session.spaceshipConfig.color.set(SpaceshipColor::Green);
}

} // namespace sfa

