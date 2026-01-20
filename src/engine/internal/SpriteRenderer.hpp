#ifndef INTERNAL_SPRITE_RENDERER_HPP
#define INTERNAL_SPRITE_RENDERER_HPP

#include "Shader.hpp"
#include "Texture.hpp"

/*
 * @file SpriteRenderer.hpp
 * @brief Abstraction class over Rendering in OpenGL
 *
 * @details The class abstracts rendering in a way, where only the configuration of the sprite has to be
 *          supplied at redner time and then the Renderer executes all OpenGL functions required to render
 *          a textured quad
 *
 *  @author Felix Hommel
 *  @date Nov 17, 2024
 */
class SpriteRenderer
{
public:
    /** Constructor / Destructor */
    SpriteRenderer(const Shader& shader);
    ~SpriteRenderer();

    /** Delete move constructor / assignment operator and leave the copy constructor / assignmetn operator at default */
    SpriteRenderer(const SpriteRenderer&) = default;
    SpriteRenderer(SpriteRenderer&&) = delete;
    SpriteRenderer& operator=(const SpriteRenderer&) = default;
    SpriteRenderer& operator=(SpriteRenderer&&) = delete;

    /** Public member functions */
    void draw(
        const Texture2D& texture,
        const glm::vec2& position,
        const glm::vec2& size = glm::vec2(10.f, 10.f),
        float rotate = 0.f,
        const glm::vec3& color = glm::vec3(1.f)
    );

private:
    /** Member variables */
    Shader m_shader;
    unsigned int m_quadVAO;
};

#endif //! INTERNAL_SPRITE_RENDERER_HPP
