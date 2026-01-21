#ifndef CORE_MENU_STATE_HPP
#define CORE_MENU_STATE_HPP

#include "Color.hpp"
#include "State.hpp"
#include "utility/Button.hpp"

namespace sfa
{

/*
 * @file MenuState.hpp
 * @brief This class represents the Menu state
 *
 * @details The class handles communication with the webserver for colors, and offers a Button to quit and
 *          a Button to play the game.
 *
 * @author Felix Hommel
 * @date Nov 17, 2024
 */
class MenuState : public State
{
public:
    /** Constructor / Destructor */
    MenuState(const Texture2D& background, const WindowInfo* windowInfo, const Mouse* mouseInfo);
    ~MenuState() override;

    /** Delete move constructor / assignment operator and leave the copy constructor / assignment operator at default */
    MenuState(const MenuState&) = delete;
    MenuState(MenuState&&) = delete;
    MenuState& operator=(const MenuState&) = delete;
    MenuState& operator=(MenuState&&) = delete;

    /** Public member functions */
    void update(float dt) override;
    void render(SpriteRenderer& renderer, TextRenderer& textRenderer) override;

    [[nodiscard]] bool getQuit() const { return m_quit; }
    [[nodiscard]] bool getPlay() const { return m_play; }
    [[nodiscard]] bool getColorRecieved() const { return m_colorRecieved; }
    [[nodiscard]] ColorType getColor() const { return m_color; }

    void resetPlay() { m_play = false; }

    void resetColorRecieved() { m_colorRecieved = false; }

private:
    /** Member variables */
    Button* m_startButton;
    Button* m_quitButton;
    ColorType m_color;

    bool m_quit;
    bool m_play;
    bool m_colorRecieved;
};

} // namespace sfa

#endif //! CORE_MENU_STATE_HPP

