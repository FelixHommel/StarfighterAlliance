#include "Projectile.hpp"

namespace sfa
{

Projectile::Projectile(const Texture2D& texture, int damage, const glm::vec2& position, const WindowInfo* windowInfo)
    : GameObject(texture, position, windowInfo), m_damage(damage)
{
    m_size = { m_windowInfo->height * 0.01f, m_windowInfo->width * 0.01f };
}

void Projectile::update(float dt)
{
    m_position.y -= movementSpeed * dt;
}

} // namespace sfa

