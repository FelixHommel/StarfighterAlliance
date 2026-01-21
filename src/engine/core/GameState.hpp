#ifndef CORE_GAME_STATE_HPP
#define CORE_GAME_STATE_HPP

#include "Color.hpp"
#include "Score.hpp"
#include "State.hpp"
#include "gameObjects/EnemyWaveGenerator.hpp"
#include "gameObjects/spaceship/Spaceship.hpp"

#include <memory>

namespace sfa
{

/*
 * @file GameState.hpp
 * @brief This class represents the Game screen
 *
 * @details This class handles the actual gameplay loop, involving everything from starting the countdown until
 *          the spaceship has lost all of it's lives.
 *
 * @author Felix Hommel
 * @date Nov 20, 2024
 */
class GameState : public State
{
public:
    /** Constructor / Destructor */
    GameState(
        const Texture2D& background,
        const WindowInfo* windowInfo,
        const Mouse* mouse,
        SpaceshipType type,
        ColorType color
    );
    ~GameState() override = default;

    /** Delete move constructor / assignment operator and leave the copy constructor / assignment operator at default */
    GameState(const GameState&) = delete;
    GameState(GameState&&) = delete;
    GameState& operator=(const GameState&) = delete;
    GameState& operator=(GameState&&) = delete;

    /** Public member functions */
    void update(float dt) override;
    void render(SpriteRenderer& renderer, TextRenderer& textRenderer) override;

    [[nodiscard]] bool getGameOver() const { return m_gameOver; }
    [[nodiscard]] Score getScore() const { return m_currentScore; }

private:
    std::shared_ptr<Spaceship> m_spaceship;
    Score m_currentScore;
    bool m_gameOver;

    bool m_paused;
    float m_blinkCounter;
    static constexpr float blinkMax{ 1.5f };

    bool m_playCountdown;
    static constexpr float countdownMax{ 4.f };
    float m_countdown;

    bool m_showStart;
    static constexpr float showStartDurationMax{ 1.f };
    float m_showStartDuration;

    std::unique_ptr<EnemyWaveGenerator> m_waveGenerator;

    void updateBlinking(float dt);
    void renderBlinking(TextRenderer& textRenderer, const std::string& text, float posX, float posY) const;

    void checkCollision();
};

} // namesapce sfa

#endif //! CORE_GAME_STATE_HPP

