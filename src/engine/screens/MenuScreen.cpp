#include "MenuScreen.hpp"

#include "core/RenderContext.hpp"
#include "utility/userInput/InputController.hpp"

namespace sfa
{

MenuScreen::MenuScreen(OnEnterFunction onEnter, OnExitFunction onExit)
    : m_onEnterFunction(onEnter)
    , m_onExitFunction(onExit)
{}

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

void MenuScreen::update(float dt, const InputController& controller)
{
}

void MenuScreen::render(const RenderContext& context)
{
}

} // namespace sfa

