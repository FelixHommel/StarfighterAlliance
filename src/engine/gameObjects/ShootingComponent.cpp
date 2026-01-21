#include "ShootingComponent.hpp"

namespace sfa
{

ShootingComponent::ShootingComponent(
    const Texture2D& texture, const glm::vec2* spaceshipPos, int damage, const WindowInfo* windowInfo
)
    : windowInfo(windowInfo), spaceshipPos(spaceshipPos), m_damage(damage), m_texture(texture), m_shotCooldown(0.f)
{}

void ShootingComponent::update(float dt)
{
    if(m_shotCooldown > 0.f)
        m_shotCooldown -= dt;
}

void ShootingComponent::shoot(bool& didShoot)
{
    if(m_shotCooldown <= 0.f)
    {
        didShoot = true;
        m_shotCooldown = ShootingComponent::shotCooldownMax;
        m_projectiles.emplace_back(
            m_texture,
            m_damage,
            glm::vec2(
                spaceshipPos->x + ((static_cast<float>(windowInfo->width) * 0.1f) / 2.f)
                    - (static_cast<float>(m_texture.getWidth()) * 0.5f),
                spaceshipPos->y
            ),
            windowInfo
        );
    }
    else
        didShoot = false;
}

} // namespace sfa

