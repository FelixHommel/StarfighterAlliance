#ifndef SFA_SRC_ENGINE_GAME_HPP
#define SFA_SRC_ENGINE_GAME_HPP

#include "core/Color.hpp"
#include "core/Score.hpp"
#include "core/State.hpp"
#include "gameObjects/spaceship/Spaceship.hpp"
#include "utility/SpriteRenderer.hpp"
#include "utility/TextRenderer.hpp"

#include <memory>
#include <stack>

namespace sfa
{

/// \brief Handles the main functionality of the game
///
/// The Game class manages the correct behavior of the game states and loads all the required resources that are needed.
///
/// \author Felix Hommel
/// \date Nov 16, 2024
class Game
{
public:
    Game(const WindowInfo* pWindowInfo, const Mouse* pMouse);
    ~Game() = default;

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    [[nodiscard]] bool getQuit() const { return m_quit; }

    /// \brief Update the State and react to eventual events that happened
    ///
    /// First run the update() function of the top most state, and then check for new events that happened and react
    /// accordingly to them.
    void update(float dt);
    void render();

private:
    const WindowInfo* m_windowInfo;
    const Mouse* m_mouse;

    std::unique_ptr<SpriteRenderer> m_renderer;
    std::unique_ptr<TextRenderer> m_textRenderer;
    std::stack<std::shared_ptr<State>> m_states;

    bool m_quit{ false };

    SpaceshipType m_spaceshipType{ SpaceshipType::x_wing };
    ColorType m_spaceshipColor{ ColorType::red };
    bool m_colorRetrieved{ false }; // NOTE: probably not needed anymore
    Score m_currentScore{};

    void loadResources();
};

} // namespace sfa

#endif //! SFA_SRC_ENGINE_GAME_HPP

