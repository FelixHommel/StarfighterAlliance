#ifndef CORE_STATE_HPP
#define CORE_STATE_HPP

#include "WindowInfo.hpp"
#include "utility/SpriteRenderer.hpp"
#include "utility/TextRenderer.hpp"
#include "utility/Texture.hpp"

namespace sfa
{

/*
 * @brief Enumeration of all possible game states
 */
enum class StateName : uint8_t
{
    Menu,
    Selection,
    Playing,
    End
};

/*
 * @brief Contains information about the mouse state
 */
struct Mouse
{
    float posX;
    float posY;
    bool isPressed;
};

/*
 * @file State.hpp
 * @brief A base class for game states
 *
 * @details This class offers an interface for the specific State objects
 *
 * @author Felix Hommel
 * @date Nov 17, 2024
 */
class State
{
public:
    /** Constructor / Destructor */
    State(const Texture2D& background, const WindowInfo* windowInfo, const Mouse* mouse, StateName name)
        : m_background(background), m_windowInfo(windowInfo), m_mouse(mouse), m_state(name)
    {}
    virtual ~State() = default;

    /** Delete move constructor / assignment operator and leave the copy constructor / assignmetn operator at default */
    State(const State&) = default;
    State(State&&) = delete;
    State& operator=(const State&) = default;
    State& operator=(State&&) = delete;

    /** Public member functiopns */
    virtual void update(float dt) = 0;
    virtual void render(SpriteRenderer& renderer, TextRenderer& textRenderer) = 0;

    [[nodiscard]] StateName getState() { return m_state; }

protected:
    /* Protected members */
    Texture2D m_background;

    const WindowInfo* m_windowInfo;
    const Mouse* m_mouse;

    StateName m_state;
};

} // namespace sfa

#endif //! CORE_STATE_HPP

