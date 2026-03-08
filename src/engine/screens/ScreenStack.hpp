#ifndef SFA_SRC_ENGINE_SCREENS_SCREEN_STACK_HPP
#define SFA_SRC_ENGINE_SCREENS_SCREEN_STACK_HPP

#include "core/RenderContext.hpp"
#include "screens/IScreen.hpp"
#include "screens/ScreenCommand.hpp"
#include "utility/userInput/InputController.hpp"

#include <memory>
#include <queue>
#include <vector>

namespace sfa
{

/// \brief Responsible for handling different screens and transitions between them.
///
/// Uses a stack primitive to represent the structure of the active screens. \ref ScreenStack has to propagate user
/// input to the screens, update the state in the individual screens, and render the relevant screens.
///
/// \author Felix Hommel
/// \date 3/3/2026
class ScreenStack
{
public:
    /// \brief Enqueue a new \ref ScreenCommand.
    ///
    /// \param command the \ref ScreenCommand detailing the action that should be taken
    void enqueueCommand(ScreenCommand command);
    /// \brief Process the enqueued commands.
    void processCommands();

    /// \brief Propagate the inputs to the screens.
    ///
    /// \param controller the \ref InputController providing input states
    void handleInput(const InputController& controller);
    /// \brief Update the screen at the top of the stack.
    ///
    /// \param dt delta time
    void update(float dt);
    /// \brief Render all the relevant screens.
    ///
    /// Relevant in this context means all screens from the top of the stack down to the first non-overlay screen.
    ///
    /// \param context the \ref RenderContext providing the context to allow rendering the screens
    void render(const RenderContext& context);

    [[nodiscard]] std::size_t size() const noexcept { return m_stack.size(); }

private:
    std::vector<std::unique_ptr<IScreen>> m_stack;
    std::queue<ScreenCommand> m_pendingCommands;

    void push(std::unique_ptr<IScreen> screen);
    void pop();
    void replace(std::unique_ptr<IScreen> screen);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_SCREENS_SCREEN_STACK_HPP

