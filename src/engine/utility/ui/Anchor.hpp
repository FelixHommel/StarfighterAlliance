#ifndef SFA_SRC_ENGINE_UTILITY_UI_ANCHOR_HPP
#define SFA_SRC_ENGINE_UTILITY_UI_ANCHOR_HPP

#include "glm/glm.hpp"

#include <cstdint>

namespace sfa
{

/// \brief 
enum class Anchor : std::uint8_t
{
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    CENTER_LEFT,
    CENTER,
    CENTER_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
};

static glm::vec2 calculateAnchorPosition(Anchor anchor, glm::vec2 containerPos, const glm::vec2& containerSize, const glm::vec2& elementSize)
{
    // NOTE: Horizontal elements
    // *_LEFT are not affected by containerPos.x changes
    switch(anchor)
    {
        case Anchor::TOP_CENTER:
        case Anchor::CENTER:
        case Anchor::BOTTOM_CENTER:
            containerPos.x += (containerSize.x - elementSize.x) * 0.5f;
            break;
        case Anchor::TOP_RIGHT:
        case Anchor::CENTER_RIGHT:
        case Anchor::BOTTOM_RIGHT:
            containerPos.x += containerSize.x - elementSize.x;
            break;
        default:
            break;
    }

    // NOTE: Vertical elements
    // TOP_* are not affected by containerPos.y changes
    switch(anchor)
    {
        case Anchor::CENTER_LEFT:
        case Anchor::CENTER:
        case Anchor::CENTER_RIGHT:
            containerPos.y += (containerSize.y - elementSize.y) * 0.5f;
            break;
        case Anchor::BOTTOM_LEFT:
        case Anchor::BOTTOM_CENTER:
        case Anchor::BOTTOM_RIGHT:
            containerPos.y += containerSize.y - elementSize.y;
            break;
        default:
            break;
    }

    return containerPos;
}

} // namespace sfa

#endif // SFA_SRC_ENGINE_UTILITY_UI_ANCHOR_HPP

