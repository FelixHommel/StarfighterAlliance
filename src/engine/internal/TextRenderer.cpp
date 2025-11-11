#include "TextRenderer.hpp"

#include "ResourceManager.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "glad/gl.h"
#include "glm/ext/matrix_clip_space.hpp"

#include <iostream>

/*
 * @brief Prepare the VertexArray and VertexBuffer to load Data later
 *
 * @param width, the width of the window where the text will be rendered
 * @param height, thw hwight of the window where the text will be rendered
*/
TextRenderer::TextRenderer(unsigned int width, unsigned int height) : m_vao {0}, m_vbo {0}, m_textShader {}
{
	m_textShader = ResourceManager::loadShader("resources/shaders/text.vert", "resources/shaders/text.frag", nullptr, "text");

	m_textShader.setMatrix4("projection", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f), true);
	m_textShader.setInteger("text", 0);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TextRenderer::~TextRenderer()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

/*
 * @brief Load a font from a file
 *
 * @param font, path to the font. Should idealy be a .fft fiel
 * @param fontSize, the size the font will be
*/
void TextRenderer::load(std::string font, unsigned int fontSize)
{
	m_characters.clear();

	FT_Library ft {};
	if(FT_Init_FreeType(&ft) != 0)
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face {};
	if(FT_New_Face(ft, font.c_str(), 0, &face) != 0)
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, fontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Load Glyph for the first 128 ASCII Characters
    for(GLubyte c = 0; c < 128; ++c)
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        unsigned int texture {0};
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<int>(face->glyph->bitmap.width), static_cast<int>(face->glyph->bitmap.rows), 0, GL_RED,
                     GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), static_cast<unsigned int>(face->glyph->advance.x)};
        m_characters.insert(std::pair<char, Character>(c, character));
    }
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

/*
 * @brief Draw text to the screen
 *
 * @param text, the text that will be drawn
 * @param x, the x position of the top left corner of the text
 * @param y, the y position of the top left corner of the text
 * @param scale, apply extra scale to the text. DEFAULT: 1
 * @param color, the color of the text. DEFAULT: glm::vec3(1.f)
*/
void TextRenderer::draw(std::string text, float x, float y, float scale, glm::vec3 color)
{
	m_textShader.use();
	m_textShader.setVector3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_vao);

	std::string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_characters[*c];

        float xpos = x + (static_cast<float>(ch.bearing.x) * scale);
        float ypos = y + (static_cast<float>(m_characters['H'].bearing.y - ch.bearing.y) * scale);

        float w = static_cast<float>(ch.size.x) * scale;
        float h = static_cast<float>(ch.size.y) * scale;
        float vertices[6][4] = {
            {	 xpos, ypos + h, 0.0f, 1.0f},
            {xpos + w,	 ypos, 1.0f, 0.0f},
            {	 xpos,	   ypos, 0.0f, 0.0f},

            {	 xpos, ypos + h, 0.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 1.0f},
            {xpos + w,	   ypos, 1.0f, 0.0f}
        };
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        //Bitshift by 6 == 2^6, advance is 1/64 of a pixel
        x += (ch.advance >> 6) * scale;
    }

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
