#ifndef GAME_OBJECTS_ENEMIES_ALIEN_HPP
#define GAME_OBJECTS_ENEMIES_ALIEN_HPP

#include "Enemy.hpp"

/*
 * @file Alien.hpp
 * @brief This class represents a single enemy
 *
 * @details all information about a single enemy is stored here, that includes for example texture, position, size.
 *
 * @author Felix Hommel
 * @date Dec 10, 2024
 */
class Alien : public Enemy
{
public:
    /** Constructor / Destructor */
    Alien(const Texture2D& texture, const glm::vec2& pos, float sizeX, const WindowInfo* windowInfo);
    ~Alien() override = default;
};

#endif //! GAME_OBJECTS_ENEMIES_ALIEN_HPP
