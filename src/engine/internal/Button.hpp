#ifndef INTERNAL_BUTTON_HPP
#define INTERNAL_BUTTON_HPP

#include "Shader.hpp"
#include "TextRenderer.hpp"

#include <functional>

/*
 * @file Button.hpp
 * @brief A abstraction class for Buttons
 *
 * @details This calss abstracts Buttons to a point where only the position and size needs to be defined
 *          and then the Button can be rendered via the member draw() function
 *
 * @author Felix Hommel
 * @date Nov 16, 2024
*/
class Button
{
	public:
		/** Constructor / Destructor */
		Button(const glm::vec2& pos, const glm::vec2& size, const glm::vec3& defaultColor, const glm::vec3& hoveredColor,
			const glm::vec3& pressedColor, const std::string& label, const Shader& shader, float pressCooldownMax = 0.25f);
		~Button();

		/** Delete move constructor / assignment operator and leave the copy constructor / assignmetn operator at default */
		Button(const Button&) = default;
		Button(Button&&) = default;
		Button& operator=(const Button&) = default;
		Button& operator=(Button&&) = default;

		/** Public member functions */
		void update(float mouseX, float mouseY, bool mousePressed, float dt = 0.f);
		void draw(TextRenderer* renderer = nullptr);

		void setOnClick(std::function<void()> callback) { m_onClick = callback; }

	private:
		/** Member variables */
		Shader m_shader;

		unsigned int m_vao;

		glm::vec2 m_position;
		glm::vec2 m_size;
		glm::vec3 m_defaultColor;
		glm::vec3 m_hoveredColor;
		glm::vec3 m_pressedColor;
		glm::vec3 m_renderColor;
		std::string m_label;

		std::function<void()> m_onClick;
        float m_pressCooldownMax;
        float m_pressCooldown;
};

#endif //!INTERNAL_BUTTON_HPP
