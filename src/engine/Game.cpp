#include "Game.hpp"

#include "core/EndState.hpp"
#include "core/GameState.hpp"
#include "core/MenuState.hpp"
#include "core/SpaceshipSelectionState.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "internal/ResourceManager.hpp"

Game::Game(const WindowInfo* windowInfo, const Mouse* mouse)
    : m_windowInfo(windowInfo)
    , m_mouse(mouse)
    , m_renderer(nullptr)
    , m_quit(false)
    , m_spaceshipType(SpaceshipType::x_wing)
    , m_spaceshipColor(ColorType::red)
    , m_colorRetrieved(false)
    , m_currentScore()
{
    loadResources();

    m_renderer = std::make_shared<SpriteRenderer>(ResourceManager::getShader("sprite"));

    m_textRenderer = std::make_shared<TextRenderer>(m_windowInfo->width, m_windowInfo->height);
    m_textRenderer->load("resources/fonts/prstartk.ttf");

    m_states.push(std::make_shared<MenuState>(ResourceManager::getTexture("background"), windowInfo, mouse));
}

Game::~Game()
{
    while(m_states.size() != 0)
        m_states.pop();
}

/*
 * @brief Update the State and react to eventual events that happened
 *
 * @details First run the update() function of the top most state, and then check for new events that happened and react accordingly to them.
*/
void Game::update(float dt)
{
    m_states.top()->update(dt);

    switch(m_states.top()->getState())
    {
        case StateName::Menu:
            {
                if(std::shared_ptr<MenuState> menu = std::dynamic_pointer_cast<MenuState>(m_states.top()))
                {
                    if(menu->getQuit())
                        m_quit = true;
                    else if(!m_colorRetrieved && menu->getColorRecieved())
                    {
                        m_spaceshipColor = menu->getColor();
                        m_colorRetrieved = true;
                    }
                    else if(menu->getPlay())
                    {
                        menu->resetPlay();
                        m_states.push(std::make_shared<SpaceshipSelectionState>(ResourceManager::getTexture("background"), m_windowInfo, m_mouse));
                    }
                    else if(glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_A) == GLFW_PRESS)
                        m_states.push(std::make_shared<EndState>(ResourceManager::getTexture("background"), m_windowInfo, m_mouse, m_currentScore));
                }
            }
            break;
        case StateName::Selection:
            {
                if(std::shared_ptr<SpaceshipSelectionState> selection = std::dynamic_pointer_cast<SpaceshipSelectionState>(m_states.top()))
                {
                    if(selection->getStartPressed())
                    {
                        m_spaceshipType = selection->getSpaceshipType();

                        m_states.pop();
                        m_states.push(std::make_shared<GameState>(ResourceManager::getTexture("background"), m_windowInfo, m_mouse, m_spaceshipType, m_spaceshipColor));
                    }
                }
            }
            break;
        case StateName::Playing:
            {
                if(std::shared_ptr<GameState> game = std::dynamic_pointer_cast<GameState>(m_states.top()))
                {
                    if(game->getGameOver())
                    {
                        m_currentScore = game->getScore();

                        m_states.pop();
                        m_states.push(std::make_shared<EndState>(ResourceManager::getTexture("background"), m_windowInfo, m_mouse, m_currentScore));
                    }
                }
            }
            break;
        case StateName::End:
            {
                if(std::shared_ptr<EndState> end = std::dynamic_pointer_cast<EndState>(m_states.top()))
                {
                    if(end->getBackButtonPressed())
                    {
                        m_states.pop();
                        m_states.push(std::make_shared<MenuState>(ResourceManager::getTexture("background"), m_windowInfo, m_mouse));
                        m_colorRetrieved = false; //reset this, so that a new color will be applied
                    }
                }
            }
            break;
    }
}

void Game::render()
{
    m_states.top()->render(*m_renderer, *m_textRenderer);
}

void Game::loadResources()
{
    glm::mat4 projection{ glm::ortho(0.f, static_cast<float>(m_windowInfo->width), static_cast<float>(m_windowInfo->height), 0.f) };

    ResourceManager::loadShader("resources/shaders/sprite.vert", "resources/shaders/sprite.frag", nullptr, "sprite");
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);

    ResourceManager::loadTexture("resources/textures/background.png", "background");
    
    ResourceManager::loadShader("resources/shaders/button.vert", "resources/shaders/button.frag", nullptr, "button");
    ResourceManager::getShader("button").use().setMatrix4("projection", projection);

    ResourceManager::loadTexture("resources/textures/Projectile.png", "Projectile");
    ResourceManager::loadTexture("resources/textures/meteorite/meteorite001.png", "Meteorite");
    ResourceManager::loadTexture("resources/textures/alien/alien001.png", "alien");
}
