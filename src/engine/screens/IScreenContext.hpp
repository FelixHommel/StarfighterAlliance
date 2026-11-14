#ifndef SFA_SRC_ENGINE_SCREENS_SCREEN_CONTEXT_HPP
#define SFA_SRC_ENGINE_SCREENS_SCREEN_CONTEXT_HPP

#include "screens/ScreenCommand.hpp"

namespace sfa
{

/// \brief Interface that defines how a screen container should behave
///
/// Needed to invert dependencies between the \ref ScreenStack and specific \ref IScreen which is needed when i.e.,
/// a screen needs to push another screen or pop the stack.
///
/// \author Felix Hommel
/// \date 5/23/2026
class IScreenContext
{
public:
    IScreenContext() = default;
    virtual ~IScreenContext() = default;

    IScreenContext(const IScreenContext&) = delete;
    IScreenContext& operator=(const IScreenContext&) = delete;
    IScreenContext(IScreenContext&&) = delete;
    IScreenContext& operator=(IScreenContext&&) = delete;

    virtual void enqueueCommand(ScreenCommand command) = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_SCREENS_SCREEN_CONTEXT_HPP

