#ifndef GAME_OBJECTS_GAME_OBJECT_HPP
#define GAME_OBJECTS_GAME_OBJECT_HPP

#include "core/WindowInfo.hpp"
#include "internal/SpriteRenderer.hpp"
#include "internal/Texture.hpp"

/*
 * @file GameObject.hpp
 * @brief Base class for all Objects that have movement in the game
 *
 * @details The GameObject class provides a interface for objects that will be moving on the screen and can interact with other objects,
 *          the update() function therefore is pure virtual because every object behaves differently, but render() will always work the same
 *          for all of them.
 *
 * @author Felix Hommel
 * @date Nov 23, 2024
*/
class GameObject
{
	public:
		/** Constructors / Destructor */
		GameObject(const WindowInfo* windowInfo) : m_windowInfo(windowInfo), m_position(), m_size(), m_rotation(0), m_velocity(), m_lives(0) {}
		GameObject(const Texture2D& texture, const WindowInfo* windowInfo)
			: m_windowInfo(windowInfo)
			, m_texture(texture)
			, m_position()
			, m_size()
			, m_rotation(0)
			, m_velocity()
			, m_lives(0)
		{}
		GameObject(const Texture2D& texture, const glm::vec2& pos, const WindowInfo* windowInfo)
			: m_windowInfo(windowInfo)
			, m_texture(texture)
			, m_position(pos)
			, m_size()
			, m_rotation(0)
			, m_velocity()
			, m_lives(0)
		{}
		virtual ~GameObject() = default;

		GameObject(const GameObject&) = default;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(const GameObject&) = default;
		GameObject& operator=(GameObject&&) = delete;

		/** Public member functions */
		virtual void update(float dt) = 0;
		virtual void render(SpriteRenderer& renderer) { renderer.draw(m_texture, m_position, m_size, m_rotation); }

		[[nodiscard]] int getLives() const { return m_lives; }
		[[nodiscard]] glm::vec2 getSize() const { return m_size; }
		[[nodiscard]] glm::vec2 getPosition() const { return m_position; }

        void loseLives(int amount) { m_lives -= amount; }

		static bool checkCollisionAABB(const GameObject& first, const GameObject& second)
		{
			bool collisionX{((first.getPosition().x + first.getSize().x) >= second.getPosition().x) &&
		                     ((second.getPosition().x + second.getSize().x) >= first.getPosition().x)};
			bool collisionY{((first.getPosition().y + first.getSize().y) >= second.getPosition().y) &&
			                 ((second.getPosition().y + second.getSize().y) >= first.getPosition().y)};

            return collisionX && collisionY;
		}

	protected:
		/** Protected members */
		const WindowInfo* m_windowInfo;

		Texture2D m_texture;
		glm::vec2 m_position;
		glm::vec2 m_size;
		float m_rotation;

		glm::vec2 m_velocity;

		int m_lives;

		/** Protected functions */
		void setPosition(const glm::vec2& pos) { m_position = pos; }
		void setPositionX(float pos) { m_position.x = pos; }
		void setPositionY(float pos) { m_position.y = pos; }
};

#endif //!GAME_OBJECTS_GAME_OBJECT_HPP
