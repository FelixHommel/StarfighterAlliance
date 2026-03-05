#include "ScreenStack.hpp"

#include "core/RenderContext.hpp"
#include "screens/IScreen.hpp"
#include "utility/userInput/InputController.hpp"

#include <utility>

namespace sfa
{

void ScreenStack::enqueueCommand(ScreenCommand command)
{
    m_pendingCommands.push(std::move(command));
}

void ScreenStack::processCommands()
{
    while(!m_pendingCommands.empty())
    {
        auto& cmd{ m_pendingCommands.front() };

        if(cmd.type == ScreenCommand::Type::Pop)
            pop();
        else if(cmd.type == ScreenCommand::Type::Push)
        {
            if(cmd.screen != nullptr)
                push(std::move(cmd.screen));
        }
        else
        {
            if(cmd.screen != nullptr)
                replace(std::move(cmd.screen));
        }

        m_pendingCommands.pop();
    }
}

void ScreenStack::handleInput(const InputController& controller)
{
    // TODO: Respect the fact where the top of the stack might be one or multiple overlay screens
    if(!m_stack.empty())
        m_stack.back()->handleInput(controller);
}

void ScreenStack::update(float dt)
{
    if(!m_stack.empty())
        m_stack.back()->update(dt);
}

void ScreenStack::render(const RenderContext& context)
{
    if(m_stack.empty())
        return;

    std::size_t startIndex{ m_stack.size() - 1 };

    // NOTE: Find the first non-overlay screen among the currently enqueued screens
    while(startIndex > 0 && m_stack[startIndex]->isOverlay())
        --startIndex;

    for(std::size_t i{ startIndex }; i < m_stack.size(); ++i)
        m_stack[i]->render(context);
}

void ScreenStack::push(std::unique_ptr<IScreen> screen)
{
    m_stack.emplace_back(std::move(screen));
    m_stack.back()->onEnter();
}

void ScreenStack::pop()
{
    m_stack.back()->onExit();
    m_stack.pop_back();
}

void ScreenStack::replace(std::unique_ptr<IScreen> screen)
{
    pop();
    push(std::move(screen));
}

} // namespace sfa

