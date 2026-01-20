#ifndef CORE_SPACESHIP_SELECTION_STATE_HPP
#define CORE_SPACESHIP_SELECTION_STATE_HPP

#include "State.hpp"
#include "gameObjects/spaceship/Spaceship.hpp"
#include "internal/Button.hpp"

#include <array>
#include <memory>

/*
 * @brief Struct to hold all data required to display a spaceship to the player
 */
struct SpaceshipData
{
    SpaceshipData(int lives, int speed, int shotPower, Texture2D texture)
        : lives(lives), speed(speed), shotPower(shotPower), texture(texture)
    {}

    int lives;
    int speed;
    int shotPower;
    Texture2D texture;
};

/*
 * @file SpaceshipSelectionState.hpp
 * @brief In this state the player chooses the Spaceship they want to play with
 *
 * @details The player can chosse between 3 spaceships, when they made a selection, they press the start button
 *          and the game will go into the Game state
 *
 * @author Felix Hommel
 * @date Nov 25, 2024
 */
class SpaceshipSelectionState : public State
{
public:
    /** Constructor / Destructor */
    SpaceshipSelectionState(const Texture2D& background, const WindowInfo* windowInfo, const Mouse* mouse);
    ~SpaceshipSelectionState() override;

    SpaceshipSelectionState(const SpaceshipSelectionState&) = default;
    SpaceshipSelectionState(SpaceshipSelectionState&&) = delete;
    SpaceshipSelectionState& operator=(const SpaceshipSelectionState&) = default;
    SpaceshipSelectionState& operator=(SpaceshipSelectionState&&) = delete;

    /** Public member functions */
    void update(float dt) override;
    void render(SpriteRenderer& renderer, TextRenderer& textRenderer) override;

    [[nodiscard]] bool getStartPressed() const { return m_startButtonPressed; }
    [[nodiscard]] SpaceshipType getSpaceshipType() const { return m_selectedType; }

private:
    /** Private members */
    Button* m_startButton;
    bool m_startButtonPressed;
    static constexpr float m_keyCooldownMax{ 0.15f };
    float m_keyCooldown;

    Texture2D m_hud;

    std::array<std::shared_ptr<SpaceshipData>, 3> m_spaceships;
    SpaceshipType m_selectedType;

    /** Private member functions */
    void loadSpaceshipData();
};

#endif //! CORE_SPACESHIP_SELECTION_STATE_HPP
