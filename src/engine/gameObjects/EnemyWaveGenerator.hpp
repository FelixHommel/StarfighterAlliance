#ifndef GAME_OBJECTS_ENEMY_WAVE_GENERATOR_HPP
#define GAME_OBJECTS_ENEMY_WAVE_GENERATOR_HPP

#include "EnemyWave.hpp"

#include <queue>
#include <vector>

/*
 * @file EnemyWaveGenerator.hpp
 * @brief This class manages all taht relates to EnemyWaves
 *
 * @details This class is responsible for spawning enemy waves and destroying them if no enemies in a wave are left
 *          or a wave reaches the bottom
 *
 * @author Felix Hommel
 * @date Dec 2, 2024
 */
class EnemyWaveGenerator
{
public:
    /** Constructor */
    EnemyWaveGenerator(const Texture2D& textureMeteorite, const Texture2D& textureAlien, const WindowInfo* windowInfo);

    /** Public member functions */
    void update(float dt);
    void render(SpriteRenderer& renderer);

    [[nodiscard]] std::vector<EnemyWave>& getEnemyWaves() { return m_activeWaves; }
    [[nodiscard]] bool isEmpty() const { return m_activeWaves.empty(); }
    [[nodiscard]] size_t getCurrentWaveCount() const { return m_activeWaves.size(); }
    [[nodiscard]] unsigned int getTotalWaveCount() const { return m_totalWaveCount; }
    [[nodiscard]] std::queue<unsigned int>& getPoints() { return m_pointQueue; }
    [[nodiscard]] float getCurrentEnemySpeed() const { return m_enemyMovementSpeed; }

    /** !! TESTING ONLY !! */
    void genWave() { spawnWave(); }

private:
    /** Member variables */
    Texture2D m_textureMeteorite;
    Texture2D m_textureAlien;
    const WindowInfo* m_windowInfo;

    float m_enemyMovementSpeed;
    std::vector<EnemyWave> m_activeWaves;
    unsigned int m_totalWaveCount;
    std::queue<unsigned int> m_pointQueue;

    /** Private member functions */
    void updateMovementSpeed();
    void spawnWave();
    void killWave(size_t index);
};

#endif //! GAME_OBJECTS_ENEMY_WAVE_GENERATOR_HPP
