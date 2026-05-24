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

    /// \brief Callback function that is called when the screen is pushed to the \ref ScreenStack
    virtual void onEnter() = 0;
    /// \brief Callback function that is called right before the screen is poped from the \ref ScreenStack
    virtual void onExit() = 0;

    /// \brief Update the screen
    ///
    /// \param dt delta time
    /// \param controller \ref InputController that can be used to query user input
    virtual void update(float dt, const InputController& controller) = 0;
    /// \brief Render the screen
    ///
    /// \param context \ref RenderContext used to render the screen
    virtual void render(const RenderContext& context) = 0;

    /// \brief Used to determine if the screen is an overlay or full screen
    virtual bool isOverlay() = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_SCREENS_I_SCREEN_HPP

