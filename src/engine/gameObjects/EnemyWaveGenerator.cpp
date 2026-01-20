#include "EnemyWaveGenerator.hpp"

EnemyWaveGenerator::EnemyWaveGenerator(
    const Texture2D& textureMeteorite, const Texture2D& textureAlien, const WindowInfo* windowInfo
)
    : m_textureMeteorite(textureMeteorite)
    , m_textureAlien(textureAlien)
    , m_windowInfo(windowInfo)
    , m_enemyMovementSpeed(1.f)
    , m_totalWaveCount(0)
{
    spawnWave();
}

void EnemyWaveGenerator::update(float dt)
{
    /** Potentially Spwan a new Wave */
    const float spaceshipHeight{ static_cast<float>(m_windowInfo->height) * 0.1f };
    const float minGap{ spaceshipHeight * 3.f };

    /** Order in the if-statement is important, otherwise could segfault because the vector could be accsessed while it is empty */
    if(m_activeWaves.empty()
       || m_activeWaves.back().getWavePositionY() - (m_activeWaves.back().getEnemyHeight() + EnemyWave::startHeight)
              > minGap)
        spawnWave();

    /** Update already existing waves */
    for(auto& x : m_activeWaves)
        x.update(m_enemyMovementSpeed * dt * 65);

    /** Potentially remove inactive waves */
    for(size_t i{ 0 }; i < m_activeWaves.size(); ++i)
        if(!m_activeWaves[i].isActive()
           || m_activeWaves[i].getWavePositionY() > static_cast<float>(m_windowInfo->height))
            killWave(i);
}

void EnemyWaveGenerator::render(SpriteRenderer& renderer)
{
    for(auto& x : m_activeWaves)
        x.render(renderer);
}

void EnemyWaveGenerator::updateMovementSpeed()
{
    // Default = 1
    if(m_totalWaveCount > 4)
        m_enemyMovementSpeed = 1.25f;
    if(m_totalWaveCount > 9)
        m_enemyMovementSpeed = 1.5f;
    if(m_totalWaveCount > 14)
        m_enemyMovementSpeed = 1.75f;
    if(m_totalWaveCount > 19)
        m_enemyMovementSpeed = 2.f;
}

void EnemyWaveGenerator::spawnWave()
{
    m_activeWaves.emplace_back(m_windowInfo, m_textureMeteorite, m_textureAlien);
    ++m_totalWaveCount;
    updateMovementSpeed();
}

void EnemyWaveGenerator::killWave(size_t index)
{
    /*
     * calculate points
     */
    size_t points{ m_activeWaves[index].getEnemyCountMax() * 10 };

    for(int i{ 0 }; i < m_activeWaves[index].getEnemies().size(); ++i)
        points -= 5;

    m_pointQueue.push(static_cast<unsigned int>(points));
    m_activeWaves.erase(m_activeWaves.begin() + static_cast<long>(index));
}
