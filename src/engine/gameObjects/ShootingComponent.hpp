#ifndef GAME_OBJECTS_SHOOTING_COMPONENT_HPP
#define GAME_OBJECTS_SHOOTING_COMPONENT_HPP

#include "core/WindowInfo.hpp"
#include "utility/Texture.hpp"
#include "spaceship/Projectile.hpp"

#include <vector>

namespace sfa
{

/*
 * @file ShootingComponent.hpp
 * @brief A component a GameObject can use in order to have the ability to fire shots
 *
 * @details The ShootingComponent is handling the shooting, which involves mainly the storing and
 *          creation of projectiles.
 *
 * @author Felix Hommel
 * @date Dec 7, 2024
 */
class ShootingComponent
{
public:
    /** Constructor */
    ShootingComponent(
        const Texture2D& texture, const glm::vec2* spaceshipPos, int damage, const WindowInfo* windowInfo
    );

    /** Public member functions */
    void update(float dt);

    void shoot(bool& didShoot);

    [[nodiscard]] std::vector<Projectile>& getProjectiles() { return m_projectiles; }

private:
    /** external variables */
    const WindowInfo* windowInfo;
    const glm::vec2* spaceshipPos;

    /** member variables */
    int m_damage;
    Texture2D m_texture;

    static constexpr float shotCooldownMax{ 0.25f };
    float m_shotCooldown;

    std::vector<Projectile> m_projectiles;
};

} // namespace sfa

#endif //! GAME_OBJECTS_SHOOTING_COMPONENT_HPP

