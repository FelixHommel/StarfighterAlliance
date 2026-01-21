#include "EnemyWave.hpp"

#include "enemies/Alien.hpp"

#include <chrono>

namespace sfa
{

EnemyWave::EnemyWave(const WindowInfo* windowInfo, const Texture2D& textureMeteorite, const Texture2D& textureAlien)
    : m_windowInfo(windowInfo)
    , m_textureMeteorite(textureMeteorite)
    , m_textureAlien(textureAlien)
    , m_enemyCount(0)
    , m_enemyCountMax(0)
    , m_isActive(true)
{
    std::random_device rd{};
    std::seed_seq ss{ static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count(
                      )),
                      rd(),
                      rd(),
                      rd() };
    gen = std::make_shared<std::mt19937>(ss);

    createWave();
}

void EnemyWave::update(float movement)
{
    bool allDead{ true };
    for(auto& m : m_enemies)
    {
        if(m.isAlive())
            allDead = false;

        m.moveY(movement);
    }

    if(allDead)
        m_isActive = false;
}

void EnemyWave::render(SpriteRenderer& renderer)
{
    for(auto& m : m_enemies)
        m.render(renderer);
}

/*
 * Generate all enemies
 * Step 1: generate the size of the enemy (SMALL: 0.05 - 0.1, MEDIUM: 0.1 - 0.15, BIG: 0.15 - 0.2)
 * Step 2: check if nextX + width would be greater then m_windowInfo->width
 *              true: set finished to true, do not add another enemy;
 *              false: continue with step 3
 * Step 3: offset it by a random amount of nextX
 * Step 4: generate an enemy and push it to the m_enemies
 * Step 5: update nextX with the position + width of this enemy
 * Step 6: back to step 1
 *
 * Make sure that there is a hole for the spaceship to fly through
 * Step 1: Pick a random index into m_enemies
 * Step 2: remove the enemy at index and decrease index by one
 * Step 3: remove the enemy at index - 1,
 * Step 4: repeat until the gap is at least m_windowInfo->width * 0.1f wide
 */
void EnemyWave::createWave()
{
    bool finished{ false };
    float nextX{ 5.f };

    while(!finished)
    {
        MeteoriteType type{ static_cast<MeteoriteType>(getRandomIntegeral(1, 3)) };
        std::pair<float, float> range{ getWidthRange(type) };
        float width{ getRandomFloating(range.first, range.second) };
        float offset{ getRandomFloating(5.f, 25.f) };
        int random{ getRandomIntegeral(1, 10) };

        if(nextX + offset + (static_cast<float>(m_windowInfo->height) * width)
           < static_cast<float>(m_windowInfo->width))
        {
            if(random == 1)
                m_enemies.push_back(Alien(m_textureAlien, glm::vec2(nextX + offset, startHeight), width, m_windowInfo));
            else
                m_enemies.push_back(
                    Meteorite(m_textureMeteorite, type, glm::vec2(nextX + offset, startHeight), width, m_windowInfo)
                );

            nextX += offset + m_enemies.back().getSize().x;
        }
        else
            finished = true;
    }

    size_t i{ getRandomIntegeral(static_cast<size_t>(2), m_enemies.size() - 3) };
    bool holeBigEnough{ false };
    int mod{ -1 };

    while(!holeBigEnough)
    {
        m_enemies.erase(m_enemies.begin() + static_cast<long>(i));

        if(i == 0 || i == (m_enemies.size() - 1))
            mod *= -1;

        i += mod;

        checkForBigEnoughHole(holeBigEnough);
    }

    m_enemyCountMax = m_enemies.size();
}

std::pair<float, float> EnemyWave::getWidthRange(MeteoriteType type)
{
    switch(type)
    {
    case MeteoriteType::SMALL:
        return std::make_pair(0.05f, 0.1f);
    case MeteoriteType::MEDIUM:
        return std::make_pair(0.1f, 0.15f);
    case MeteoriteType::BIG:
        return std::make_pair(0.15f, 0.2f);
    }
}

void EnemyWave::checkForBigEnoughHole(bool& flag)
{
    float spaceshipWidth{ (static_cast<float>(m_windowInfo->width) * 0.1f) * 1.5f };

    if(m_enemies[0].getPosition().x > spaceshipWidth)
    {
        flag = true;
        return;
    }

    for(size_t i{ 0 }; i < m_enemies.size() - 1; ++i)
    {
        if(m_enemies[i + 1].getPosition().x - (m_enemies[i].getPosition().x + m_enemies[i].getSize().x)
           > spaceshipWidth)
        {
            flag = true;
            return;
        }
    }
}

} // namespace sfa

