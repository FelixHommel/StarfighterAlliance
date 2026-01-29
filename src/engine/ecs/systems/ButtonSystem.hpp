#ifndef SFA_SRC_ENGINE_ECS_SYSTEMS_BUTTON_SYSTEM_HPP
#define SFA_SRC_ENGINE_ECS_SYSTEMS_BUTTON_SYSTEM_HPP

#include "ecs/ComponentRegistry.hpp"

#include <glm/glm.hpp>

namespace sfa
{

/// \brief The \ref ButtonSystem is responsible for updating button entities.
///
/// \author Felix Hommel
/// \date 1/29/2026
class ButtonSystem
{
public:
    /// \brief Update the state of the buttons.
    ///
    /// Updating the button state involves checking if they are not hovered, hovered, and pressed.
    ///
    /// \param registry The \ref ComponentRegistry with the entities.
    /// \param dt delta time.
    /// \param mousePos the position of the mouse pointer at the time of the update.
    /// \param mousePressed if the left mouse button is pressed or not at the time of the update.
    static void update(ComponentRegistry& registry, float dt, const glm::vec2& mousePos, bool mousePressed);
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_SYSTEMS_BUTTON_SYSTEM_HPP

