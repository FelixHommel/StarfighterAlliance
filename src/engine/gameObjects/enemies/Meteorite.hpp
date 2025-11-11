#ifndef GAMEOBJECTS_ENEMIES_METEORITE_HPP
#define GAMEOBJECTS_ENEMIES_METEORITE_HPP

#include "Enemy.hpp"

enum class MeteoriteType : uint8_t
{
	SMALL = 1,
	MEDIUM = 2,
	BIG = 3
};

/*
 * @file Meteorite.hpp
 * @brief This class represents a single enemy
 *
 * @details all information about a single enemy is stored here, that includes for example texture, position, size.
 *
 * @author Felix Hommel
 * @date Nov 27, 2024
*/
class Meteorite : public Enemy
{
	public:
		/** Constructor / Destructor */
		Meteorite(const Texture2D& texture, MeteoriteType type, const glm::vec2& pos, const WindowInfo* windowInfo);
		Meteorite(const Texture2D& texture, MeteoriteType type, const glm::vec2& pos, float sizeX, const WindowInfo* windowInfo);
        ~Meteorite() override = default;
};

#endif //!GAMEOBJECTS_ENEMIES_METEORITE_HPP
