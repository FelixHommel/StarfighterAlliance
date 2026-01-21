#include "SpriteRenderer.hpp"

#include "Shader.hpp"

#include "glad/gl.h"
#include "glm/ext/matrix_transform.hpp"

#include <array>

/*
 * @brief Setup a quad in a vertex array
 *
 * @param shader, Shader that will be used to render this quad
 */
SpriteRenderer::SpriteRenderer(const Shader& shader) : m_shader(shader), m_quadVAO(0)
{
    unsigned int VBO{};
    std::array<float, 6 * 4> vertices{ // pos      // tex
                                       0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                                       0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_quadVAO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &m_quadVAO);
}

/*
 * @brief Draw a textured quad to the screen
 *
 * @details The order of actions within the funtion is important, especially the translation, rotation and scale order.
 *          1. translate -- bring the quad to the correct position
 *          2. rotate -- rotate the quad to the correct angle
 *          3. scale -- scale the quad to the correct size
 *          Note that the actual cooridantes of all the quads are the same. The position on the screen
 *          is computed with matrix transformations, which achieves the same visual results.
 *
 * @param texture, the texture of the quad
 * @param position, the position of the quad on screen
 * @param size, the size of the quad on screen. DEFAULT: glm::vec2(10.f, 10.f)
 * @param rotate, the rotation of the quad. DEFAULT: 0.f
 * @param color, the color of the quad. If a color and texture is supplied the two will be blended to produce the final look. DEFAULT: glm::vec3(1.f)
 */
void SpriteRenderer::draw(
    const Texture2D& texture, const glm::vec2& position, const glm::vec2& size, float rotate, const glm::vec3& color
)
{
    m_shader.use();
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position, 0.0f));

    /** with the translation the rotation will happen around the center point of the sprite instead of top elft corner */
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    m_shader.setMatrix4("model", model);
    m_shader.setVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}
