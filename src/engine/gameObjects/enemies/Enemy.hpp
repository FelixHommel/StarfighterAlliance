#ifndef GAME_OBJECTS_ENEMY_HPP
#define GAME_OBJECTS_ENEMY_HPP

#include "gameObjects/GameObject.hpp"

class Enemy : public GameObject
{
public:
    Enemy(const Texture2D& texture, const glm::vec2& pos, const WindowInfo* windowInfo, int damage)
        : GameObject(texture, pos, windowInfo), m_damage(damage), m_isAlive(true)
    {}
    ~Enemy() override = default;

    void update(float dt) override {}

    void moveY(float movement) { m_position.y += movement; }
    void setTexture(const Texture2D& texture) { m_texture = texture; }

    [[nodiscard]] bool isAlive() const { return m_isAlive; }
    [[nodiscard]] int getDamage() const { return m_damage; }

protected:
    /** Protected member varaibles */
    int m_damage;
    bool m_isAlive;
};

#endif //! GAME_OBJECTS_ENEMY_HPP
