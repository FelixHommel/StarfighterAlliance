#include "MenuState.hpp"

#include "Color.hpp"
#include "internal/JSONReader.hpp"
#include "internal/ResourceManager.hpp"

MenuState::MenuState(const Texture2D& background, const WindowInfo* windowInfo, const Mouse* mouse)
    : State(background, windowInfo, mouse, StateName::Menu)
    , m_startButton(nullptr)
    , m_quitButton(nullptr)
    , m_color()
    , m_quit(false)
    , m_play(false)
    , m_colorRecieved(false)
{
    m_startButton = new Button(
        glm::vec2(m_windowInfo->width * 0.1, m_windowInfo->height * 0.3),
        glm::vec2(m_windowInfo->width * 0.2, m_windowInfo->height * 0.1),
        glm::vec3(158.f / 255.f, 80.f / 255.f, 250.f / 255.f),
        glm::vec3(100.f / 255.f, 33.f / 255.f, 177.f / 255.f),
        glm::vec3(81.f / 255.f, 8.f / 255.f, 166.f / 255.f),
        "Start",
        ResourceManager::getShader("button")
    );
    m_quitButton = new Button(
        glm::vec2(m_windowInfo->width * 0.1, m_windowInfo->height * 0.3 + m_windowInfo->height * 0.25),
        glm::vec2(m_windowInfo->width * 0.2, m_windowInfo->height * 0.1),
        glm::vec3(158.f / 255.f, 80.f / 255.f, 250.f / 255.f),
        glm::vec3(100.f / 255.f, 33.f / 255.f, 177.f / 255.f),
        glm::vec3(81.f / 255.f, 8.f / 255.f, 166.f / 255.f),
        "Quit",
        ResourceManager::getShader("button")
    );

    m_startButton->setOnClick([&play = m_play]() { play = true; });
    m_quitButton->setOnClick([&quit = m_quit]() { quit = true; });
}

MenuState::~MenuState()
{
    delete m_startButton;
    delete m_quitButton;
}

void MenuState::update(float dt)
{
    m_startButton->update(m_mouse->posX, m_mouse->posY, m_mouse->isPressed);
    m_quitButton->update(m_mouse->posX, m_mouse->posY, m_mouse->isPressed);

    m_color = ColorType::red;
}

void MenuState::render(SpriteRenderer& renderer, TextRenderer& textRenderer)
{
    renderer.draw(m_background, glm::vec2(0.f, 0.f), glm::vec2(m_windowInfo->width, m_windowInfo->height));
    m_startButton->draw(&textRenderer);
    m_quitButton->draw(&textRenderer);
    textRenderer.draw("Starfighter", m_windowInfo->width * 0.5f, m_windowInfo->height * 0.2f, 3.f);
    textRenderer.draw("Alliance", m_windowInfo->width * 0.5f, m_windowInfo->height * 0.3f, 3.f);
}
