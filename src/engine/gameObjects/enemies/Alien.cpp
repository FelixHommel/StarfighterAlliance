#include "Alien.hpp"

/*
 * Constructor with sizeX, will make a rectangular meteorite
*/
Alien::Alien(const Texture2D& texture, const glm::vec2& pos, float sizeX, const WindowInfo* windowInfo)
    : Enemy(texture, pos, windowInfo, 2)
{
    m_size = { static_cast<float>(m_windowInfo->height) * sizeX, static_cast<float>(m_windowInfo->height) * 0.1f };
}
