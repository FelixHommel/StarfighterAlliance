#ifndef GAME_OBJECTS_ENEMY_WAVE_HPP
#define GAME_OBJECTS_ENEMY_WAVE_HPP

#include "enemies/Enemy.hpp"
#include "enemies/Meteorite.hpp"

#include <random>
#include <vector>

namespace sfa
{

/*
 * @file EnemyWave.hpp
 * @brief This class is responsible for all things that affect the wave as a whole.
 *
 * @details The class first has to create a new wave that follows all the constraints a wave must obide by.
 *          After that, it has to make sure that they wall move at the same time, and keep track wehther or not
 *          there are still active enemies in the wave
 *
 * @author Felix Hommel
 * @date Nov 29, 2024
 */
class EnemyWave
{
public:
    /** Constructor */
    EnemyWave(const WindowInfo* windowInfo, const Texture2D& textureMeteorite, const Texture2D& textureAlien);

    /** Public member functions */
    void update(float movement);
    void render(SpriteRenderer& renderer);

    [[nodiscard]] bool isActive() const { return m_isActive; }
    [[nodiscard]] size_t getEnemyCountMax() const { return m_enemyCountMax; }
    [[nodiscard]] float getWavePositionY() const { return m_enemies[0].getPosition().y; }
    [[nodiscard]] float getEnemyHeight() const { return m_enemies[0].getSize().y; }
    [[nodiscard]] std::vector<Enemy>& getEnemies() { return m_enemies; }

    void checkForBigEnoughHole(bool& flag);

    /** Public accessable constants */
    static constexpr float startHeight{ 50.f };

private:
    /** Private member variables */
    const WindowInfo* m_windowInfo;
    Texture2D m_textureMeteorite;
    Texture2D m_textureAlien;

    std::vector<Enemy> m_enemies;
    size_t m_enemyCount;
    size_t m_enemyCountMax;
    bool m_isActive;

    std::shared_ptr<std::mt19937> gen;
    /** Random floating point number, [min, max] (inclusive) */
    template<typename T>
    T getRandomFloating(T min, T max)
    {
        return std::uniform_real_distribution<T>{ min, max }(*gen);
    }
    /** Random  integer, [min, max] (inclusive) */
    template<typename T>
    T getRandomIntegeral(T min, T max)
    {
        return std::uniform_int_distribution<T>{ min, max }(*gen);
    }

    /** Private member functions */
    void createWave();
    static std::pair<float, float> getWidthRange(MeteoriteType type);
};

} // namespace sfa

#endif //! GAME_OBJECTS_ENEMY_WAVE_HPP

