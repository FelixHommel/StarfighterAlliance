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
    void enqueueCommand(ScreenCommand command);
    void processCommands();

    void handleInput(const InputController& controller);
    void update(float dt);
    void render(const RenderContext& context);

private:
    std::vector<std::unique_ptr<IScreen>> m_stack;
    std::queue<ScreenCommand> m_pendingCommands;

    void push(std::unique_ptr<IScreen> screen);
    void pop();
    void replace(std::unique_ptr<IScreen> screen);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_SCREENS_SCREEN_STACK_HPP

