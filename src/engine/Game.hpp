#ifndef GAME_HPP
#define GAME_HPP

#include "core/Score.hpp"
#include "core/State.hpp"
#include "gameObjects/spaceship/Spaceship.hpp"
#include "internal/SpriteRenderer.hpp"
#include "internal/TextRenderer.hpp"

#include <memory>
#include <stack>

/*
 * @file Game.hpp
 * @brief Handles the main functionality of the game
 *
 * @details The Game class manages the correct behavior of the game states and loads all the required
 *          resources that are needed
 *
 * @author Felix Hommel
 * @date Nov 16, 2024
 */
class Game
{
public:
    /** Constructor / Destructor */
    Game(const WindowInfo* windowInfo, const Mouse* mouse);
    ~Game();

    /** Delete move constructor / assignment and leave the copy constructor / assignment to default */
    Game(const Game&) = default;
    Game& operator=(const Game&) = default;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    /** Public member functions */
    void update(float dt);
    void render();

    [[nodiscard]] bool getQuit() const { return m_quit; }

private:
    /** Member variables */
    const WindowInfo* m_windowInfo;
    const Mouse* m_mouse;

    std::shared_ptr<SpriteRenderer> m_renderer;
    std::shared_ptr<TextRenderer> m_textRenderer;

    std::stack<std::shared_ptr<State>> m_states;
    bool m_quit;

    SpaceshipType m_spaceshipType;
    ColorType m_spaceshipColor;
    bool m_colorRetrieved;
    Score m_currentScore;

    /** Private member functions */
    void loadResources();
};

#endif //! GAME_HPP
