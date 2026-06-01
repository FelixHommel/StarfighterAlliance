#include "SelectionScreen.hpp"

#include "config/SpaceshipColor.hpp"
#include "config/SpaceshipType.hpp"
#include "core/RenderContext.hpp"
#include "core/resourceManagement/ResourceContext.hpp"
#include "ecs/ECSUtility.hpp"
#include "ecs/UIHelper.hpp"
#include "ecs/components/SpriteComponent.hpp"
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
#include "utility/json/JsonDocument.hpp"
#include "utility/userInput/InputController.hpp"
#include "utility/userInput/InputEvent.hpp"

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <cstddef>
#include <filesystem>
#include <format>
#include <memory>
#include <string_view>
#include <type_traits>
#include <utility>

namespace
{

constexpr sfa::EntityID ROOT_ENTITY{ 1 };
constexpr sfa::EntityID CENTER_AREA{ 2 };
constexpr sfa::EntityID CENTER_ROW{ 3 };
constexpr sfa::EntityID BOTTOM_BAR{ 4 };
constexpr sfa::EntityID SPACESHIP_PREVIEW_ENTITY{ 5 };
constexpr sfa::EntityID LEFT_BUTTON_ENTITY{ 6 };
constexpr sfa::EntityID RIGHT_BUTTON_ENTITY{ 7 };
constexpr sfa::EntityID LEFT_BOTTOM_BUTTON_ENTITY{ 8 };
constexpr sfa::EntityID RIGHT_BOTTOM_BUTTON_ENTITY{ 9 };

constexpr auto ROOT_SPACING{ 20.f };
constexpr auto ROOT_PADDING{ glm::vec2(20.f) };
constexpr auto CENTER_AREA_SIZE_WIDTH_OFFSET{ 40 };
constexpr auto CENTER_AREA_SIZE_HEIGHT_OFFSET{ 160 };
constexpr auto CENTER_ROW_WIDTH_OFFSET{ 40 };
constexpr auto CENTER_ROW_HEIGHT{ 400 };
constexpr auto CENTER_ROW_SPACING{ 40.f };
constexpr auto CENTER_ROW_PADDING{ glm::vec2(40.f) };
constexpr auto BOTTOM_BAR_WINDOW_OFFSET{ 40.f };
constexpr auto BOTTOM_BAR_SIZE_Y{ 100.f };
constexpr auto BOTTOM_BAR_PADDING{ glm::vec2(20.f) };
constexpr auto BOTTOM_BAR_SPACING_OFFSET{ 520.f };
constexpr auto BUTTON_COLOR{ glm::vec3(0.33f) };
constexpr auto SELECTION_BUTTON_SIZE{ glm::vec2(120.f, 70.f) };
constexpr auto BOTTOM_BAR_BUTTON_SIZE{ glm::vec2(220.f, 70.f) };

constexpr auto SPACESHIP_CONFIG_SCHEMA_PATH{ SFA_ROOT "resources/config/spaceship_config_schema.json" };
constexpr auto SPACESHIP_STAT_VALUE_FALLBACK{ 0u };

[[nodiscard]] constexpr std::filesystem::path getSpaceshipConfigPath(sfa::SpaceshipType type) noexcept
{
    return std::format("{}resources/config/{}.json", SFA_ROOT, type);
}

[[nodiscard]] constexpr std::filesystem::path getSpaceshipTexturePath(
    sfa::SpaceshipType type, sfa::SpaceshipColor color
) noexcept
{
    return std::format("{}resources/textures/{}/{}_{}.png", SFA_ROOT, type, type, color);
}

} // namespace

namespace sfa
{

SelectionScreen::SelectionScreen(
    IScreenContext& context, GameSession& session, const Viewport& viewport, ResourceContext& resources
)
    : m_screenContext(context), m_session(session)
{
    loadSpaceshipData(resources);

    createCorePanelsUI(viewport);
    createSpaceshipPreviewUI(resources);
    createBottomButtonsUI(viewport);
}

void SelectionScreen::onEnter()
{
    if(m_onEnterFunction)
        m_onEnterFunction();
}

void SelectionScreen::onExit()
{
    if(m_onExitFunction)
        m_onExitFunction();
}

void SelectionScreen::update(float dt, const InputController& controller)
{
    LayoutSystem::update(m_registry);
    UITransformSystem::update(m_registry);
    ButtonSystem::update(
        m_registry, dt, controller.mousePosition(), controller.isMousePressed(MouseButton::Left) == InputAction::Press
    );
}

void SelectionScreen::render(const RenderContext& context)
{
    context.uiRenderer.render(m_registry, context.window.viewport());
}

/// \brief Create the root UI panels
void SelectionScreen::createCorePanelsUI(const Viewport& viewport)
{
    // ROOT
    m_registry.addComponent<UITransformComponent>(
        ::ROOT_ENTITY,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = static_cast<glm::vec2>(viewport),
        }
    );
    m_registry.addComponent<UIHierarchyComponent>(
        ::ROOT_ENTITY,
        {
            .parent = NULL_ENTITY,
            .children = { ::CENTER_AREA, ::BOTTOM_BAR },
    }
    );
    m_registry.addComponent<UILayoutComponent>(
        ::ROOT_ENTITY,
        {
            .type = UILayoutComponent::Type::Vertical,
            .spacing = ::ROOT_SPACING,
            .padding = ::ROOT_PADDING,
        }
    );

    // CENTER AREA
    m_registry.addComponent<UITransformComponent>(
        ::CENTER_AREA,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = glm::vec2(
                viewport.width - ::CENTER_AREA_SIZE_WIDTH_OFFSET, viewport.height - ::CENTER_AREA_SIZE_HEIGHT_OFFSET
            ),
        }
    );
    m_registry.addComponent<UIHierarchyComponent>(
        ::CENTER_AREA,
        {
            .parent = ::ROOT_ENTITY,
            .children = { ::CENTER_ROW },
        }
    );
    m_registry.addComponent<UILayoutComponent>(
        ::CENTER_AREA,
        {
            .type = UILayoutComponent::Type::Vertical,
            .spacing = 0.f,
            .padding = glm::vec2(0.f),
        }
    );

    // CENTER ROW
    m_registry.addComponent<UITransformComponent>(
        ::CENTER_ROW,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = glm::vec2(viewport.width - ::CENTER_ROW_WIDTH_OFFSET, ::CENTER_ROW_HEIGHT),
        }
    );
    m_registry.addComponent<UIHierarchyComponent>(
        ::CENTER_ROW,
        {
            .parent = ::CENTER_AREA,
            .children = { ::LEFT_BUTTON_ENTITY, ::SPACESHIP_PREVIEW_ENTITY, ::RIGHT_BUTTON_ENTITY },
    }
    );
    m_registry.addComponent<UILayoutComponent>(
        ::CENTER_ROW,
        {
            .type = UILayoutComponent::Type::Horizontal,
            .spacing = ::CENTER_ROW_SPACING,
            .padding = ::CENTER_ROW_PADDING,
        }
    );
}

void SelectionScreen::createSpaceshipPreviewUI(const ResourceContext& resources)
{
    auto pic{ resources.getTexture(m_spaceships.at(static_cast<std::size_t>(m_currentSelection))->textureName) };

    // PREVIEW SPRITE
    m_registry.addComponent<UITransformComponent>(
        ::SPACESHIP_PREVIEW_ENTITY,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = pic->size(),
        }
    );
    m_registry.addComponent<UIHierarchyComponent>(
        ::SPACESHIP_PREVIEW_ENTITY,
        {
            .parent = ::CENTER_ROW,
            .children = {},
        }
    );
    m_registry.addComponent<sfa::UILayoutElementComponent>(
        ::SPACESHIP_PREVIEW_ENTITY,
        {
            .preferredSize = pic->size(),
            .flexGrow = 0.f,
        }
    );
    m_registry.addComponent<SpriteComponent>(
        ::SPACESHIP_PREVIEW_ENTITY,
        {
            .texture = pic.get(),
            .size = pic->size(),
            .color = glm::vec3(0.f),
            .renderLayer = 0,
        }
    );

    // SIDE BUTTONS
    ui::addButton(
        m_registry,
        ::LEFT_BUTTON_ENTITY,
        ::CENTER_ROW,
        ::SELECTION_BUTTON_SIZE,
        "<",
        ::BUTTON_COLOR,
        [&idx = m_currentSelection, maxSpaceships = m_spaceships.size()] {
            spdlog::info("LEFT pressed");
            int newIndex{ static_cast<int>(idx) - 1 };
            idx = static_cast<SpaceshipType>((newIndex < 0) ? maxSpaceships - 1 : newIndex);
        }
    );
    ui::addButton(
        m_registry,
        ::RIGHT_BUTTON_ENTITY,
        ::CENTER_ROW,
        ::SELECTION_BUTTON_SIZE,
        ">",
        ::BUTTON_COLOR,
        [&idx = m_currentSelection, maxSpaceships = m_spaceships.size()] {
            spdlog::info("RIGHT pressed");
            idx = static_cast<SpaceshipType>((static_cast<int>(idx) + 1) % maxSpaceships);
        }
    );
}

void SelectionScreen::createBottomButtonsUI(const Viewport& viewport)
{
    const auto windowWidth{ static_cast<float>(viewport.width) };

    // BOTTOM BAR
    m_registry.addComponent<UITransformComponent>(
        ::BOTTOM_BAR,
        {
            .localPosition = glm::vec2(0.f),
            .worldPosition = glm::vec2(0.f),
            .size = glm::vec2(windowWidth - ::BOTTOM_BAR_WINDOW_OFFSET, ::BOTTOM_BAR_SIZE_Y),
        }
    );

    m_registry.addComponent<UIHierarchyComponent>(
        ::BOTTOM_BAR,
        {
            .parent = ::ROOT_ENTITY,
            .children = { ::LEFT_BOTTOM_BUTTON_ENTITY, ::RIGHT_BOTTOM_BUTTON_ENTITY },
    }
    );

    m_registry.addComponent<UILayoutComponent>(
        ::BOTTOM_BAR,
        {
            .type = UILayoutComponent::Type::Horizontal,
            .spacing = windowWidth - ::BOTTOM_BAR_SPACING_OFFSET,
            .padding = ::BOTTOM_BAR_PADDING,
        }
    );

    // BOTTOM BUTTONS
    ui::addButton(
        m_registry,
        ::LEFT_BOTTOM_BUTTON_ENTITY,
        ::BOTTOM_BAR,
        ::BOTTOM_BAR_BUTTON_SIZE,
        "BACK",
        ::BUTTON_COLOR,
        [&ctx = m_screenContext] {
            spdlog::info("SelectionScreen: BACK pressed");
            ctx.enqueueCommand({ .type = ScreenCommand::Type::Pop, .screen = nullptr });
        }
    );
    ui::addButton(
        m_registry,
        ::RIGHT_BOTTOM_BUTTON_ENTITY,
        ::BOTTOM_BAR,
        ::BOTTOM_BAR_BUTTON_SIZE,
        "SELECT",
        ::BUTTON_COLOR,
        [&ctx = m_screenContext] {
            spdlog::info("SelectionScreen: SELECT pressed");
            ctx.enqueueCommand(
                { .type = ScreenCommand::Type::Push, .screen = nullptr }
            ); // FIXME: Push the correct screen
        }
    );
}

/// \brief Load the data needed to show the player the available spaceships and their stats
///
/// \param resources the \ref ResourceContext needed to load the preview images
void SelectionScreen::loadSpaceshipData(ResourceContext& resources)
{
    for(std::underlying_type_t<SpaceshipType> i{ 0 }; i < std::to_underlying(SpaceshipType::SPACESHIP_TPYE_MAX); ++i)
    {
        const auto type{ static_cast<SpaceshipType>(i) };
        const auto spaceshipName{ std::format("{}_{}", type, m_session.spaceshipConfig.color.get()) };

        if(resources.containsTexture(spaceshipName))
            continue;

        JsonDocument d{ ::getSpaceshipConfigPath(type) };

        resources.requestResourceBlocking(
            ResourceContext::TextureLoadRequest{
                .name = spaceshipName,
                .filepath = ::getSpaceshipTexturePath(type, m_session.spaceshipConfig.color.get()),
            }
        );

        const bool validationResult{ d.validate(::SPACESHIP_CONFIG_SCHEMA_PATH) };
        if(!validationResult)
        {
            spdlog::warn(
                "Spaceship config for '{}' failed to validate agsinst the schema located at '{}'. Using placeholder values.",
                spaceshipName,
                ::SPACESHIP_CONFIG_SCHEMA_PATH
            );
        }

        m_spaceships.at(i) = std::make_unique<SpaceshipData>(
            validationResult ? d.get<unsigned int>("health") : ::SPACESHIP_STAT_VALUE_FALLBACK,
            validationResult ? d.get<unsigned int>("speed") : ::SPACESHIP_STAT_VALUE_FALLBACK,
            validationResult ? d.get<unsigned int>("damage") : ::SPACESHIP_STAT_VALUE_FALLBACK,
            std::move(spaceshipName)
        );
    }
}

} // namespace sfa

