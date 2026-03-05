#ifndef SFA_SRC_ENGINE_SCREENS_I_SCREEN_HPP
#define SFA_SRC_ENGINE_SCREENS_I_SCREEN_HPP

#include "core/RenderContext.hpp"
#include "utility/userInput/InputController.hpp"

namespace sfa
{

/// \brief Interface representing the feature set that a game screen has to be able to handle.
///
/// \author Felix Hommel
/// \date 3/3/2026
class IScreen
{
public:
    IScreen() = default;
    virtual ~IScreen() = default;

    IScreen(const IScreen&) = delete;
    IScreen& operator=(const IScreen&) = delete;
    IScreen(IScreen&&) noexcept = default;
    IScreen& operator=(IScreen&&) noexcept = default;

    virtual void onEnter() = 0;
    virtual void onExit() = 0;

    virtual void handleInput(const InputController& controller) = 0;
    virtual void update(float dt) = 0;
    virtual void render(const RenderContext& context) = 0;

    virtual bool isOverlay() = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_SCREENS_I_SCREEN_HPP

