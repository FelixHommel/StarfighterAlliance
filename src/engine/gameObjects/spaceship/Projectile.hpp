#ifndef GAME_OBJECTS_SPACESHIP_PROJECTILE_HPP
#define GAME_OBJECTS_SPACESHIP_PROJECTILE_HPP

#include "gameObjects/GameObject.hpp"

/*
 * @file Projectile.hpp
 * @brief This class represents a single projectile that can be fired by the Player
 *
 * @details The Projectile only active task is to fly upwards (negative y direction on screen).
 *          On this path it is possible for a projectile to collide with an enemy, which will result
 *          in the enemy loosing health and the projectile beeing destroyed. If a projectile reaches
 *          the top of the screen, it will be destroyed automatically.
 *
 * @author Felix Hommel
 * @date Dec 7, 2024
 */
class Projectile : public GameObject
{
public:
    /** Constructor / Destructor */
    Projectile(const Texture2D& texture, int damage, const glm::vec2& position, const WindowInfo* windowInfo);
    ~Projectile() override = default;

    /** Public member functions */
    void update(float dt) override;

    [[nodiscard]] int getDamage() const { return m_damage; }

    /** Constant valuse */
    static constexpr float movementSpeed{ 800.f };

private:
    /** Member varaibles */
    int m_damage;
};

#endif //! GAME_OBJECTS_SPACESHIP_PROJECTILE_HPP
