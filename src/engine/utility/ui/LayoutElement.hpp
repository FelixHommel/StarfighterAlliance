#ifndef SFA_SRC_ENGINE_UTILITY_UI_LAYOUT_ELEMENT_HPP
#define SFA_SRC_ENGINE_UTILITY_UI_LAYOUT_ELEMENT_HPP

#include "glm/glm.hpp"
#include "utility/TextRenderer.hpp"

namespace sfa
{

/// \brief Base class for all elements that can be fit into layout containers
///
/// \author Felix Hommel
/// \date 1/23/2026
class LayoutElement
{
public:
    LayoutElement() = default;
    virtual ~LayoutElement() = default;

    LayoutElement(const LayoutElement&) = delete;
    LayoutElement(LayoutElement&&) = delete;
    LayoutElement& operator=(const LayoutElement&) = delete;
    LayoutElement& operator=(LayoutElement&&) = delete;

    [[nodiscard]] virtual glm::vec2 position() const = 0;
    [[nodiscard]] virtual glm::vec2 size() const = 0;

    virtual void setPosition(const glm::vec2& pos) = 0;
    virtual void setSize(const glm::vec2& size) = 0;

    virtual void update(float dt) = 0;
    virtual void render(TextRenderer* textRenderer) = 0;
};

} // namespace sfa

#endif // !SFA_SRC_ENGINE_UTILITY_UI_LAYOUT_ELEMENT_HPP

