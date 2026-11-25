#ifndef SFA_SRC_ENGINE_ENTITIES_COMPONENTS_TRANSFORM_HPP
#define SFA_SRC_ENGINE_ENTITIES_COMPONENTS_TRANSFORM_HPP

#include <glm/glm.hpp>

namespace sfa
{

/// \brief Transform infomration for entities.
///
/// \author Felix Hommel
/// \date 11/18/2026
struct Transform
{
    glm::vec2 position;
    glm::vec2 scale;
    float rotation{ 0.f };
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_ENTITIES_COMPONENTS_TRANSFORM_HPP

