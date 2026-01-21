#include "GameState.hpp"

#include "utility/ResourceManager.hpp"

#include "GLFW/glfw3.h"

namespace sfa
{

GameState::GameState(
    const Texture2D& background, const WindowInfo* windowInfo, const Mouse* mouse, SpaceshipType type, ColorType color
)
    : State(background, windowInfo, mouse, StateName::Playing)
    , m_spaceship(std::make_shared<Spaceship>(type, color, windowInfo))
    , m_currentScore{ 0, 0 }
    , m_gameOver(false)
    , m_paused(true)
    , m_blinkCounter(0.f)
    , m_playCountdown(false)
    , m_countdown(countdownMax)
    , m_showStart(false)
    , m_showStartDuration(showStartDurationMax)
    , m_waveGenerator(
          std::make_unique<EnemyWaveGenerator>(
              ResourceManager::getTexture("Meteorite"), ResourceManager::getTexture("alien"), m_windowInfo
          )
      )
{}

void GameState::update(float dt)
{
    if(m_paused)
    {
        updateBlinking(dt);

        if(glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            m_blinkCounter = 0.f;
            m_paused = false;
            m_playCountdown = true;
        }
    }
    else if(m_playCountdown)
    {
        m_countdown -= dt;

        if(m_countdown <= 0.f)
        {
            m_playCountdown = false;
            m_countdown = 0.f;
            m_showStart = true;
        }
    }
    else if(m_showStart)
    {
        m_showStartDuration -= dt;

        if(m_showStartDuration <= 0.f)
            m_showStart = false;
    }
    else if(m_spaceship->getLives() < 0)
    {
        updateBlinking(dt);

        if(glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_SPACE) == GLFW_PRESS)
            m_gameOver = true;
    }
    else
    {
        if(glfwGetKey(m_windowInfo->window.get(), GLFW_KEY_P) == GLFW_PRESS)
            m_paused = true;

        m_spaceship->update(dt);

        auto& projectiles{ m_spaceship->getProjectiles() };
        for(size_t i{ 0 }; i < projectiles.size();)
        {
            projectiles[i].update(dt);

            /** remove the projectile if it is out of the screen area */
            if(projectiles[i].getPosition().y + projectiles[i].getSize().y < 0)
                projectiles.erase(projectiles.begin() + static_cast<long>(i));
            else
                ++i;
        }

        m_waveGenerator->update(dt);
        m_currentScore.wave = m_waveGenerator->getTotalWaveCount();

        checkCollision();

        while(!m_waveGenerator->getPoints().empty())
        {
            m_currentScore.score += m_waveGenerator->getPoints().back();
            m_waveGenerator->getPoints().pop();
        }
    }
}

void GameState::render(SpriteRenderer& renderer, TextRenderer& textRenderer)
{
    renderer.draw(m_background, glm::vec2(0.f, 0.f), glm::vec2(m_windowInfo->width, m_windowInfo->height));

    m_spaceship->render(renderer);
    m_waveGenerator->render(renderer);

    for(auto& x : m_spaceship->getProjectiles())
        x.render(renderer);

    std::stringstream ssScore;
    ssScore << "Points: " << m_currentScore.score;
    textRenderer.draw(
        ssScore.str(), static_cast<float>(m_windowInfo->width) * 0.01f, static_cast<float>(m_windowInfo->height) * 0.01f
    );

    std::stringstream ssWave;
    ssWave << "Wave  : " << m_currentScore.wave;
    textRenderer.draw(
        ssWave.str(), static_cast<float>(m_windowInfo->width) * 0.01f, static_cast<float>(m_windowInfo->height) * 0.05f
    );

    std::stringstream ssLives;
    ssLives << "Lives: " << std::to_string(m_spaceship->getLives());
    textRenderer.draw(
        ssLives.str(), static_cast<float>(m_windowInfo->width) * 0.89f, static_cast<float>(m_windowInfo->height) * 0.01f
    );

    if(m_paused)
        renderBlinking(
            textRenderer,
            "PRESS ENTER TO PLAY",
            static_cast<float>(m_windowInfo->width) * 0.265f,
            static_cast<float>(m_windowInfo->height) * 0.45f
        );
    else if(m_playCountdown)
    {
        /** Cast to int removes floating portion */
        textRenderer.draw(
            std::to_string(static_cast<int>(m_countdown)),
            static_cast<float>(m_windowInfo->width) * 0.48f,
            static_cast<float>(m_windowInfo->height) * 0.5f,
            2.f
        );
    }
    else if(m_showStart)
        textRenderer.draw(
            "START",
            static_cast<float>(m_windowInfo->width) * 0.43f,
            static_cast<float>(m_windowInfo->height) * 0.5f,
            2.f
        );
    else if(m_spaceship->getLives() < 0)
    {
        renderBlinking(
            textRenderer,
            "GAME OVER!",
            static_cast<float>(m_windowInfo->width) * 0.39f,
            static_cast<float>(m_windowInfo->height) * 0.45f
        );
        renderBlinking(
            textRenderer,
            "Press space to continue",
            static_cast<float>(m_windowInfo->width) * 0.215f,
            static_cast<float>(m_windowInfo->height) * 0.55f
        );
    }
}

void GameState::updateBlinking(float dt)
{
    m_blinkCounter += dt;
    if(m_blinkCounter > blinkMax)
        m_blinkCounter = 0.f;
}

void GameState::renderBlinking(TextRenderer& textRenderer, const std::string& text, float posX, float posY) const
{
    if(m_blinkCounter < (blinkMax / 2.f))
        textRenderer.draw(text, posX, posY, 2.f);
}

void GameState::checkCollision()
{
    /** projectile - meteorite */
    for(size_t waveIdx{ 0 }; waveIdx < m_waveGenerator->getCurrentWaveCount(); ++waveIdx)
    {
        EnemyWave& wave{ m_waveGenerator->getEnemyWaves()[waveIdx] };

        for(size_t i{ 0 }; i < wave.getEnemies().size();)
        {
            Enemy& meteorite{ wave.getEnemies()[i] };

            bool hit{ false };
            int damage{ 0 };
            for(size_t proIdx{ 0 }; proIdx < m_spaceship->getProjectiles().size();)
            {
                if(GameObject::checkCollisionAABB(m_spaceship->getProjectiles()[proIdx], meteorite))
                {
                    hit = true;
                    damage = m_spaceship->getProjectiles()[proIdx].getDamage();
                    m_spaceship->getProjectiles().erase(
                        m_spaceship->getProjectiles().begin() + static_cast<long>(proIdx)
                    );
                    break;
                }
                else
                    ++proIdx;
            }

            if(hit)
                meteorite.loseLives(damage);

            if(meteorite.getLives() < 0)
                wave.getEnemies().erase(wave.getEnemies().begin() + static_cast<long>(i));
            else
                ++i;
        }
    }

    /** Spaceship - meteorite */
    for(size_t waveIdx{ 0 }; waveIdx < m_waveGenerator->getCurrentWaveCount(); ++waveIdx)
    {
        EnemyWave& wave{ m_waveGenerator->getEnemyWaves()[waveIdx] };

        for(size_t i{ 0 }; i < wave.getEnemies().size();)
        {
            Enemy& meteorite{ wave.getEnemies()[i] };

            if(GameObject::checkCollisionAABB(*m_spaceship, meteorite))
            {
                m_spaceship->loseLives(meteorite.getDamage());
                wave.getEnemies().erase(wave.getEnemies().begin() + static_cast<long>(i));
            }
            else
                ++i;
        }
    }
}

} // namespace sfa

