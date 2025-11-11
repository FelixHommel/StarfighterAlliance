#ifndef GAME_OBJECTS_MOVEMENT_COMPONENT_HPP
#define GAME_OBJECTS_MOVEMENT_COMPONENT_HPP

#include "glm/ext/vector_float2.hpp"

/*
 * @file MovementComponent.hpp
 * @brief Handle Movement of GameObjects
 *
 * @details Moves the spaceship according to the input offset as long as that wouldnt move it out of bounds
 *
 * @author Felix Hommel
 * @date Nov 24, 2024
*/ 
class MovementComponent
{
	public:
        /** Constructor */
		MovementComponent(glm::vec2* obj, const glm::vec2& size, float maxWidth, float maxHeight);

        /** Public member functions */
		void move(const glm::vec2& offset);
		void moveX(float offset) { move({offset, 0.f}); }
		void moveY(float offset) { move({0.f, offset}); }

	private:
        /* 
        *  Accessor for the GameObjects position variable
        *  !!CAREFUL, modifies the actual GameObject position!!
        */
		glm::vec2* pos;

        /** Member variables */
		glm::vec2 m_size;

		float m_maxWidth;
		float m_maxHeight;
};

#endif //!GAME_OBJECTS_MOVEMENT_COMPONENT_HPP
