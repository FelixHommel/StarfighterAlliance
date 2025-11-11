#include "Meteorite.hpp"

/*
 * Constructor without sizeX, will make a square meteorite
*/
Meteorite::Meteorite(const Texture2D& texture, MeteoriteType type, const glm::vec2& pos, const WindowInfo* windowInfo)
    :Enemy(texture, pos, windowInfo, static_cast<int>(type))
{
    switch(type)
    {
        case MeteoriteType::SMALL:
            m_size = { static_cast<float>(m_windowInfo->height) * 0.1f, static_cast<float>(m_windowInfo->height) * 0.1f };
            break;
        case MeteoriteType::MEDIUM:
            m_size = { static_cast<float>(m_windowInfo->height) * 0.15f, static_cast<float>(m_windowInfo->height) * 0.1f };
            break;
        case MeteoriteType::BIG:
            m_size = { static_cast<float>(m_windowInfo->height) * 0.2f, static_cast<float>(m_windowInfo->height) * 0.1f };
            break;
    };

}

/*
 * Constructor with sizeX, will make a rectangular meteorite
*/
Meteorite::Meteorite(const Texture2D& texture, MeteoriteType type, const glm::vec2& pos, float sizeX, const WindowInfo* windowInfo)
    :Enemy(texture, pos, windowInfo, static_cast<int>(type))
{
    m_size = { static_cast<float>(m_windowInfo->height) * sizeX, static_cast<float>(m_windowInfo->height) * 0.1f };
}
