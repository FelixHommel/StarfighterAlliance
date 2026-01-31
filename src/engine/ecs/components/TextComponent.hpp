#ifndef SFA_SRC_ENGINE_ECS_COMPONENTS_TEXT_COMPONENT_HPP
#define SFA_SRC_ENGINE_ECS_COMPONENTS_TEXT_COMPONENT_HPP

#include "ecs/components/IComponent.hpp"

#include "glm/glm.hpp"

#include <string>

namespace sfa
{

/// \brief Allow entities to have Text properties.
///
/// Give text field, scale, color, and renderLayer to an entity.
///
/// \author Felix Hommel
/// \date 1/26/2026
struct TextComponent : public IComponent
{
    std::string content;
    glm::vec2 offset{ glm::vec2(0.f) };
    float scale{ 1.f };
    glm::vec3 color{ glm::vec3(1.f) };
    unsigned int renderLayer{ 0 };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ECS_COMPONENTS_TEXT_COMPONENT_HPP

