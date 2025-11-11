#include "MovementComponent.hpp"

#include <stdexcept>

MovementComponent::MovementComponent(glm::vec2* obj, const glm::vec2& size, float maxWidth, float maxHeight)
    : pos(obj)
    , m_size(size)
    , m_maxWidth(maxWidth)
    , m_maxHeight(maxHeight)
{}

/*
 * @brief Move the position of the GameObject bei offset
 *
 * @param offset, a vec2 offset that is supposed to be applied to the position
 *
 * @throws std::runtime_error, when the position is a nullptr
*/
void MovementComponent::move(const glm::vec2& offset)
{
    if(pos == nullptr)
        throw std::runtime_error("[ERROR] MovementComponent::pos is a nullptr");

    if(0 <= pos->x + offset.x && pos->x + offset.x <= m_maxWidth - m_size.x)
        pos->x += offset.x;
    else if(0 > pos->x + offset.x)
        pos->x = 0;
    else if(pos->x + offset.x > m_maxWidth - m_size.x)
        pos->x = m_maxWidth - m_size.x;
    
    if(0 <= pos->y + offset.y && pos->y + offset.y <= m_maxHeight - m_size.y)
        pos->y += offset.y;
    else if(0 > pos->y + offset.y)
        pos->y = 0;
    else if(pos->y + offset.y > m_maxHeight - m_size.y)
        pos->y = m_maxHeight - m_size.y;
}
