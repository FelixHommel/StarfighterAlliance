#include "MenuScreen.hpp"

#include "config/SpaceshipColor.hpp"
#include "core/RenderContext.hpp"
#include "screens/GameSession.hpp"
#include "screens/IScreenContext.hpp"
#include "utility/IWindow.hpp"
#include "utility/userInput/InputController.hpp"

namespace sfa
{

MenuScreen::MenuScreen(IScreenContext& context, IWindow& window, GameSession& session)
    : m_screenContext(context)
    , m_window(window)
    , m_session(session)
    , m_onEnterFunction([&session = m_session]() { MenuScreen::makeColorRequest(session); })
{
    // TODO: Create UI
    // TODO: Send color request to the color server and save future in \p session
}

void MenuScreen::onEnter()
{
    if(m_onEnterFunction)
        m_onEnterFunction();
}

void MenuScreen::onExit()
{
    if(m_onExitFunction)
        m_onExitFunction();
}

void MenuScreen::update(float dt, const InputController& controller) {}

void MenuScreen::render(const RenderContext& context) {}

void MenuScreen::makeColorRequest(GameSession& session)
{
    // TODO: Implement actual request
    if(false) // NOLINT
        session.spaceshipConfig.color.set(SpaceshipColor::Green);
}

} // namespace sfa

