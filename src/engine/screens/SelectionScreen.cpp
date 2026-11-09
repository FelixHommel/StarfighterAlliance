#include "SelectionScreen.hpp"

#include "config/SpaceshipColor.hpp"
#include "config/SpaceshipType.hpp"
#include "core/RenderContext.hpp"
#include "core/resourceManagement/ResourceContext.hpp"
#include "screens/GameSession.hpp"
#include "screens/IScreenContext.hpp"
#include "screens/SelectionScreen.hpp"
#include "utility/IWindow.hpp"
#include "utility/json/JsonReader.hpp"
#include "utility/userInput/InputController.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <format>
#include <memory>
#include <stdexcept>
#include <utility>

namespace
{

constexpr auto SPACESHIP_CONFIG_SCHEMA_PATH{ SFA_ROOT "resources/config/spaceship_config_schema.json" };

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
    IScreenContext& context, GameSession& session, [[maybe_unused]] const Viewport& viewport, ResourceContext& resources
)
    : m_screenContext(context), m_session(session)
{
    loadSpaceshipData(resources);
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

void SelectionScreen::update(float dt, const InputController& controller) {}

void SelectionScreen::render(const RenderContext& context) {}

/// \brief Load the data needed to show the player the available spaceships and their stats
///
/// \param resources the \ref ResourceContext needed to load the preview images
void SelectionScreen::loadSpaceshipData(ResourceContext& resources)
{
    for(std::underlying_type_t<SpaceshipType> i{ 0 }; i < std::to_underlying(SpaceshipType::SPACESHIP_TPYE_MAX); ++i)
    {
        const auto spaceshipType{ static_cast<SpaceshipType>(i) };
        const auto spaceshipColor{ m_session.spaceshipConfig.color.get() };
        const auto spaceshipName{ std::format("{}_{}", spaceshipType, spaceshipColor) };

        if(resources.containsTexture(spaceshipName))
            continue;

        resources.requestResourceBlocking(
            ResourceContext::TextureLoadRequest{
                .name = spaceshipName,
                .filepath = ::getSpaceshipTexturePath(spaceshipType, spaceshipColor),
            }
        );

        if(const auto parsedConfig{ sfa::json::parseJson(::getSpaceshipConfigPath(spaceshipType)) };
           parsedConfig.has_value())
        {
            const auto config{ *parsedConfig };

            m_spaceships.at(i) = std::make_unique<SpaceshipData>(
                config["health"].get<unsigned int>(),
                config["speed"].get<unsigned int>(),
                config["damage"].get<unsigned int>(),
                spaceshipName
            );
        }
        else
        {
            spdlog::critical(
                "Spaceship config for '{}' failed to validate agsinst the schema located at '{}'",
                spaceshipName,
                ::SPACESHIP_CONFIG_SCHEMA_PATH
            );

            throw std::runtime_error("Couldn't load spaceship config");
        }
    }
}

} // namespace sfa

