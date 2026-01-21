#include "SpaceshipSelectionState.hpp"

#include "utility/JSONReader.hpp"
#include "utility/ResourceManager.hpp"

namespace sfa
{

SpaceshipSelectionState::SpaceshipSelectionState(
    const Texture2D& background, const WindowInfo* windowInfo, const Mouse* mouse
)
    : State(background, windowInfo, mouse, StateName::Selection)
    , m_startButton(nullptr)
    , m_startButtonPressed(false)
    , m_keyCooldown(0)
    , m_selectedType(SpaceshipType::x_wing)
{
    m_startButton = new Button(
        glm::vec2(m_windowInfo->width * 0.4, m_windowInfo->height * 0.85),
        glm::vec2(m_windowInfo->width * 0.2, m_windowInfo->height * 0.1),
        glm::vec3(158.f / 255.f, 80.f / 255.f, 250.f / 255.f),
        glm::vec3(100.f / 255.f, 33.f / 255.f, 177.f / 255.f),
        glm::vec3(81.f / 255.f, 8.f / 255.f, 166.f / 255.f),
        "Start",
        ResourceManager::getShader("button"),
        0.5f
    );
    m_startButton->setOnClick([&start = m_startButtonPressed]() { start = true; });

    m_hud = ResourceManager::loadTexture("resources/textures/selection_hud.png", "hud");

    loadSpaceshipData();
}

SpaceshipSelectionState::~SpaceshipSelectionState()
{
    delete m_startButton;
}

void SpaceshipSelectionState::update(float dt)
{
    m_startButton->update(m_mouse->posX, m_mouse->posY, m_mouse->isPressed, dt);

    if(m_keyCooldown <= 0)
    {
        if(glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            m_selectedType = static_cast<SpaceshipType>((static_cast<int>(m_selectedType) + 1) % 3);
            m_keyCooldown = m_keyCooldownMax;
        }
        else if(glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            /** Unfortunately C++ doesn't do modulo with negative numbers the way I need it to be. e.g. -1 % 3 = -1 in C++ */
            int newIndex{ static_cast<int>(m_selectedType) - 1 };
            m_selectedType = static_cast<SpaceshipType>((newIndex < 0) ? m_spaceships.size() - 1 : newIndex);
            m_keyCooldown = m_keyCooldownMax;
        }
    }

    if(m_keyCooldown > 0)
        m_keyCooldown -= dt;
}

void SpaceshipSelectionState::render(SpriteRenderer& renderer, TextRenderer& textRenderer)
{
    renderer.draw(m_background, { 0.f, 0.f }, { m_windowInfo->width, m_windowInfo->height });
    renderer.draw(m_hud, { 0.f, 0.f }, { m_windowInfo->width, m_windowInfo->height });
    renderer.draw(
        m_spaceships.at(static_cast<int>(m_selectedType))->texture,
        { m_windowInfo->width * 0.4f, m_windowInfo->height * 0.4f },
        { m_windowInfo->width * 0.2f, m_windowInfo->height * 0.2f }
    );

    m_startButton->draw(&textRenderer);

    textRenderer.draw("Speed", m_windowInfo->width * 0.125f, m_windowInfo->height * 0.675f);
    textRenderer.draw("Lives", m_windowInfo->width * 0.815f, m_windowInfo->height * 0.675f);
    textRenderer.draw("Shot power", m_windowInfo->width * 0.655f, m_windowInfo->height * 0.158f);

    textRenderer.draw(
        std::to_string(m_spaceships.at(static_cast<int>(m_selectedType))->speed),
        m_windowInfo->width * 0.125f,
        m_windowInfo->height * 0.715f
    );
    textRenderer.draw(
        std::to_string(m_spaceships.at(static_cast<int>(m_selectedType))->lives),
        m_windowInfo->width * 0.865f,
        m_windowInfo->height * 0.715f
    );
    textRenderer.draw(
        std::to_string(m_spaceships.at(static_cast<int>(m_selectedType))->shotPower),
        m_windowInfo->width * 0.75f,
        m_windowInfo->height * 0.2f
    );

    textRenderer.draw("<", m_windowInfo->width * 0.35f, m_windowInfo->height * 0.48f, 2.f);
    textRenderer.draw(">", m_windowInfo->width * 0.625f, m_windowInfo->height * 0.48f, 2.f);
}

/*
 * @brief Initialize all 3 spaceships
 */
void SpaceshipSelectionState::loadSpaceshipData()
{
    JSONReader json;

    for(int i{ 0 }; i < m_spaceships.size(); ++i)
    {
        std::string typeString{ Spaceship::spaceshipTypeToString(static_cast<SpaceshipType>(i)) };
        json.open("resources/config/" + typeString + ".json");

        auto lives{ json.get<int>("lives") };
        auto speed{ json.get<int>("speed") };
        auto shotPower{ json.get<int>("shotPower") };

        if(!lives.has_value() || !speed.has_value() || !shotPower.has_value())
            throw std::runtime_error("[ERROR] reading data for " + typeString);

        std::stringstream ssName;
        ssName << typeString << "_base.png";
        std::stringstream ssPath;
        ssPath << "resources/textures/" << typeString << "/" << ssName.str();

        m_spaceships.at(i) = std::make_shared<SpaceshipData>(
            lives.value(),
            speed.value(),
            shotPower.value(),
            ResourceManager::loadTexture(ssPath.str().c_str(), ssName.str())
        );
    }
}

} // namespace sfa

